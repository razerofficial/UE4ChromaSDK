# ChromaSDKPlugin - Unreal Engine 4 (UE4) Blueprint library to control lighting for (Razer Chroma)

**Table of Contents**

* [Frameworks supported](#frameworks-supported)
* [Packaging](#packaging)
* [Examples](#examples)
* [Getting Started](#getting-started)

<a name="frameworks-supported"></a>
## Frameworks supported
- Windows ChromaSDK (32-bit)
- Windows ChromaSDK (64-bit)

<a name="packaging"></a>
## Packaging

1 Copy `ChromaSDKPlugin` to the `ProjectFolder\Plugins\ChromaSDKPlugin` folder

2 Open the `Plugins Browser tab` using the `Edit->Plugins` menu item

![image_5](/images/image_5.png)

3 Make sure the `ChromeSDKPlugin` is enabled

![image_6](/images/image_6.png)

<a name="examples"></a>
## Examples

### Example Level

The Example Level displays a UI `Blueprint Widget` with buttons that have `onclick events` that use the `ChromaSDKPlugin`.

1 Open the level in `Content/Levels/ExampleLevel`

![image_1](/images/image_1.png)

2 Open the UI `Blueprint Widget` in `Content/Blueprints/ExampleWidget_BP`

![image_2](/images/image_2.png)

3 Open the `Level Blueprint` with the `Blueprints->Open Level Blueprint` menu item

![image_3](/images/image_3.png)

4 The main features of the example level will be labelled with comment blocks

![image_4](/images/image_4.png)

The example begins at `Event BeginPlay` which is executed on launch.
The `Setup UI` custom event loads the `Blueprint Widget`.
If the `Windows` platform is detected, the `ChromaSDKInit` custom event is called.

![image_7](/images/image_7.png)

The `SetupUI` custom event loads the UI `Blueprint Widget` and saves a `widget` variable to be referenced by later UI controls.
The `Blueprint Widget` is added to the viewport and then the `SetupUIButtons` custom event is called.

![image_8](/images/image_8.png)

The `SetupUIButtons` custom event uses the `widget` variable to access the named buttons to bind `onclicked` events.
When a button is clicked the `Event` delegate is invoked.
Since the `Event` delegate doesn't take parameters, an unnamed custom event is used to prepare the button parameters and invoke the `SetAllDeviceStaticColor` custom event.
`FLinearColor` is used to mimize the number of steps when passing colors to events and the SDK.
This same setup is used for all the static color buttons.

![image_9](/images/image_9.png)

The `SetupUIButtons` custom event calls other custom events to setup the clear button, the random button, and the animation buttons.

![image_10](/images/image_10.png)

The example ends at `Event EndPlay` which executes when the game exits or `ESC` is pressed in the editor.
If the `Windows` platform is detected, the `ChromaSDKUninit` custom event is called.

![image_11](/images/image_11.png)

<a name="getting-started"></a>
## Getting Started
