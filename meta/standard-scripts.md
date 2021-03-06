# Standard Behavior Scripts

## Logging scripts (2D)

### `LocationScript`
A script displaying a `NodeTransform`'s translation `T` and rotation `R` coordinates.

### `StatsScript`
A script displaying statistics like `FPS` (frames per second), `CPU` (CPU usage percentage between frames), and `RAM` (physical main memory usage between frames).

### `TextConsoleScript`
A script displaying a console/terminal with a fixed number of rows and colums.

## Movement and Orientation

### `CharacterMotorScript`
A script handling a `NodeTransform`'s translation based on the keyboard's input.

### `FPSInputControllerScript`
A script combining a `NodeTransform`'s translation and rotation via a `CharacterMotorScript` and `MouseLookScript`.

### `ManhattanInputControllerScript`
A script combining a `NodeTransform`'s translation and rotation via a `ManhattanMotorScript` and `MouseLookScript`.

### `ManhattanMotorScript`
A script handling a `NodeTransform`'s translation (object coodinate space axis-aligned) based on the keyboard's input.

### `MouseLookScript`
A script handling a `NodeTransform`'s rotation based on the mouse's input.

### `RotationScript`
A script rotating a `NodeTransform`'s around one of its principal axes.

## Rendering

### `RenderModeScript`
A script for switching between different rendering modes and layers.

## Scene

### `SwitchSceneScript`
A script for switching the current scene.
