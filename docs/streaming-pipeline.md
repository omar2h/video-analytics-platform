# Streaming Pipeline

## Purpose

The streaming pipeline turns a configured RTSP source into a current image available to the Qt Quick UI without running FFmpeg work in the GUI thread.

## Connect sequence

```mermaid
sequenceDiagram
    participant UI as QML
    participant CM as CameraManagementViewModel
    participant SM as StreamingManager
    participant SS as StreamingSession
    participant SW as StreamingWorker
    participant FS as FFmpegStreamingService
    participant C as RTSP camera

    UI->>CM: connectSelectedCamera()
    CM->>SM: startStreaming(camera)
    SM->>SS: start(config)
    SS->>SW: queued start(url)
    Note over SW,FS: camera streaming thread
    SW->>FS: stream(url)
    FS->>C: avformat_open_input()
    C-->>FS: RTSP media stream
    FS->>FS: find video stream and open decoder
    FS-->>SW: connected()
    SW-->>SS: stateChanged(Connected)
```

## Decode and frame delivery

```mermaid
sequenceDiagram
    participant FS as FFmpegStreamingService
    participant FC as FFmpegFrameConverter
    participant SW as StreamingWorker
    participant FE as FrameExchange
    participant SS as StreamingSession
    participant VM as CameraStreamViewModel
    participant LVM as LiveMonitoringViewModel
    participant IP as VideoFrameProvider
    participant QML as QML Image

    loop packet read/decode
        FS->>FS: av_read_frame(AVPacket)
        FS->>FS: avcodec_send_packet()
        FS->>FS: avcodec_receive_frame(AVFrame)
        FS->>FC: convert(AVFrame)
        FC-->>FS: QImage
        FS-->>SW: frameReady(image)
        SW->>FE: publish(image, decode time)
        SW-->>SS: frameUpdated()
        SS-->>VM: frameUpdated()
        VM->>FE: snapshot()
        FE-->>VM: latest FrameSnapshot
        VM-->>LVM: currentFrameChanged()
        LVM->>IP: setImage(camera ID, image)
        VM-->>QML: frameRevisionChanged()
        QML->>IP: request image://video/id?rev=N
        IP-->>QML: QImage
    end
```

## FFmpeg resources

`FFmpegStreamingService` owns the resources used by a streaming attempt:

| Resource | Role |
| --- | --- |
| `AVFormatContext` | RTSP input, stream metadata, packet read |
| `AVCodecContext` | video decoder state |
| `AVPacket` | reusable compressed-packet container |
| `AVFrame` | reusable decoded-frame container |
| `SwsContext` | pixel-format conversion, owned by `FFmpegFrameConverter` |

The service initializes these in stream setup and releases them through its cleanup methods before an attempt ends.

## Latest-frame contract

The live-view contract is intentionally not “deliver every decoded frame.” It is “make the newest decoded frame observable.”

```text
producer: publish frame 101 → publish frame 102 → publish frame 103
consumer: snapshot() receives the latest available snapshot
```

`FrameSnapshot.revision` identifies a new published frame. `decodeTime` enables future frame-age/latency monitoring. `valid` distinguishes an empty exchange from a real image.

## Connection states

```mermaid
stateDiagram-v2
    [*] --> Disconnected
    Disconnected --> Connecting: start
    Connecting --> Connected: FFmpeg connected
    Connecting --> Reconnecting: initialization failure
    Connected --> Reconnecting: network failure
    Reconnecting --> Connected: reconnect succeeds
    Reconnecting --> Error: retry budget exhausted
    Connected --> Disconnected: cancellation
    Reconnecting --> Disconnected: cancellation
    Error --> Disconnected: explicit stop/new lifecycle
```

## Statistics

`FFmpegStreamingService` updates codec, resolution, FPS, bitrate, received packet count, and decoded frame count. It emits statistics at approximately one-second intervals, and `StreamingSession` forwards them to `CameraStreamViewModel` for QML display.

## Recording

Recording is not implemented. A future recording subsystem should consume encoded packets or a dedicated media pipeline, not frames retrieved through the GUI-facing `FrameExchange`.

