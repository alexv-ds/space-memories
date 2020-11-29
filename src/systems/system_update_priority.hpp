#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WorldUpdate,
  WindowEventHandler,
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
  WindowEventHandlerClear,
  Init,
  MapLoader,
  ExitIfWindowClosed
};


};
