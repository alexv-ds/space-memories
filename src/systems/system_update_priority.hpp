#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WorldUpdate,
  ListenWindowEvents,
  InputServiceUpdate,
  WASDRawInputMovable,
  DenseIntersectionControll,
  ExitAfter10sec,
  CameraAutoPrefferedSize,
  PixelPerfectCameraSize,
  CameraFixedUnitSize,
  SpaceIconInitialise,
  AddSpriteFrameeAnimationIfNeeded,
  SpriteFrameAnimation,
  RenderBegin,
  DefaultRenderSystem,
  CameraPillarbox,
  DebugRenderRegionFrame,
  RenderEnd,
  SFMLRenderWindow,
  InputServiceClear,
  Init,
  MapLoader,
  CloseWindowEventHandler,
  ExitIfWindowClosed
};


};
