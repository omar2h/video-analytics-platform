# Changelog

All notable project changes are documented here. Entries correspond to repository tags.

## [1.10.0] — Initial live analytics overlays

- Add an asynchronous analytics session with latest-frame and latest-result mailboxes.
- Integrate YOLOX detection into the first configured desktop camera.
- Add QML detection overlays with coordinate scaling and stale-result expiration.
- Attach stream-run identifiers to decoded frames.
- Preserve cancellation across queued starts and reconnection attempts.
- Correct streaming-worker destruction and QML ownership.
- Safely retire camera view models when sessions are removed.
- Add analytics and session-removal regression tests.

## [1.9.0] — Standalone object detection

- Add detection value types and the detector interface.
- Implement YOLOX-Nano inference through ONNX Runtime.
- Add image preprocessing, prediction decoding, and class-aware NMS.
- Add a command-line image detection demo.

## [1.8.2] — Thread-safe recording command handling

- Add mutex-protected start/stop recording command ingress.
- Consume recording commands from the active streaming loop.
- Keep recorder state and FFmpeg output-resource access on the streaming thread.

## [1.8.1] — Keyframe-aware MP4 recording

- Defer MP4 output initialization until a video keyframe is received.
- Add `Starting` and `Error` handling to the recorder lifecycle.
- Clean up pending and active recording state on stop.

## [1.8.0] — MP4 recording milestone

- Add FFmpeg packet-remux recording to MP4 output.
- Add start/stop recording controls.
- Propagate recording state, duration, and output-file information to QML.
- Stop active recording during stream cleanup.

## [1.7.1] — Streaming architecture refinements

- Encapsulate latest-frame access through the session API.
- Refine streaming-thread synchronization and frame-exchange integration.

## [1.7.0] — Latest-frame rendering architecture

- Introduce `FrameExchange` and revision-driven live monitoring updates.

## [1.6.0] — Stream statistics

- Add codec, resolution, bitrate, packet, and decoded-frame statistics.

## [1.5.0] – [1.5.1] — Monitoring UI refinement

- Add state-aware stream controls and improve camera details/status presentation.

## [1.4.0] — Live monitoring UI

- Add the live monitoring page and camera-grid presentation.

## [1.3.0] — Session architecture

- Add streaming-manager integration and per-camera session ownership.

## [1.2.0] — Automatic reconnect

- Add reconnect policy and stream retry behavior.

## [1.1.0] — Threaded streaming

- Move streaming work off the GUI thread and add graceful shutdown behavior.

## [1.0.0] — End-to-end RTSP rendering

- Deliver an RTSP-to-QML path through FFmpeg decode and Qt rendering.

## [0.2.0] – [0.10.0] — Foundation milestones

- Camera CRUD, validation, SQLite persistence, RTSP connection, decoding, and frame conversion.

