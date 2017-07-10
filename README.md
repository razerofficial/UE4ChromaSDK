# ChromaSDKPlugin - Unreal Engine 4 (UE4) Blueprint library to control lighting for (Razer Chroma)

**Table of Contents**

* [Frameworks supported](#frameworks-supported)
* [Prerequisites](#prerequisites)
* [Engine Versions](#engine-versions)
* [Usage](#usage)
* [Assets](#assets)
* [Blueprint objects](#blueprint-objects)
* [Blueprint functions](#blueprint-functions)
* [Examples](#examples)
* [Getting Started](#getting-started)

<a name="frameworks-supported"></a>
## Frameworks supported
- Windows ChromaSDK (32-bit)
- Windows ChromaSDK (64-bit)

<a name="prerequisites"></a>
## Prerequisites

- Install [Visual Studio](https://www.visualstudio.com/)
- Install [Windows Software Development Kit (SDK) for Windows 8.1](https://developer.microsoft.com/en-us/windows/downloads/windows-8-1-sdk)
- Install [Windows 10 SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- Install `Templates->Other Languages->Visual C++->Visual C++ 2015 Tools for Windows Desktop` which can be installed through the `Visual Studio New Project Dialog`

![image_42](images/image_42.png)

<a name="engine-versions"></a>
## Engine Versions

The `Plugin` was created in UE4 `4.16`.

**Note: To compile the plugin on versions before `4.16`, adjust the comments in the module build files.**

### ChromaSDKPlugin.Build.cs

The runtime module - `Plugins\ChromaSDKPlugin\Source\ChromaSDKPlugin\ChromaSDKPlugin.Build.cs`

```c++
using UnrealBuildTool;

public class ChromaSDKPlugin : ModuleRules
{
	public ChromaSDKPlugin(TargetInfo Target) //4.15 or lower
	//public ChromaSDKPlugin(ReadOnlyTargetRules Target) : base(Target) // 4.16 or better
```

### ChromaSDKEditor.Build.cs

The editor module - `Plugins\ChromaSDKPlugin\Source\ChromaSDKEditor\ChromaSDKEditor.Build.cs`

```c++
using UnrealBuildTool;

public class ChromaSDKEditor : ModuleRules
{
	public ChromaSDKEditor(TargetInfo Target) //4.15 or lower
	//public ChromaSDKEditor(ReadOnlyTargetRules Target) : base(Target) // 4.16 or better
```

<a name="usage"></a>
## Usage

1 Copy `ChromaSDKPlugin` to the `ProjectFolder\Plugins\ChromaSDKPlugin` folder

2 Open the `Plugins Browser tab` using the `Edit->Plugins` menu item

![image_5](images/image_5.png)

3 Make sure the `ChromeSDKPlugin` is enabled

![image_6](images/image_6.png)

<a name="assets"></a>
## Assets

In the `Content Browser` under the `Add New->Miscellaneous` menu item is where animation assets can be created for `ChromaSDKPluginAnimation1DObject` and `ChromaSDKPluginAnimation2DObject`.

![image_33](images/image_33.png)

Hovering `ChromaSDK Animations` will show a `Play` or `Stop` thumbnail. Clicking the `Play` thumbnail will play the animation effect when in `play-mode` or `edit-mode`.

![image_34](images/image_34.png)

Right-click a `ChromaSDK Animation` and select `Edit` to open the custom `details view`.

![image_35](images/image_35.png)

The `details view` sets the device type and allows the color animation frames to be edited.
`1D` animations support `ChromaLink`, `Headset`, and `Mousepad` devices.
`2D` animations support `Keyboard`, `Keypad`, and `Mouse` devices.
The `device preview` shows a grid and clicking the elements will set the color from the color wheel.
The `Curve` adjusts the timings for each animation frame.

![image_36](images/image_36.png)

`ChromaSDK Animation` objects can be referenced from blueprints.
Create a variable of object type `ChromaSDKPluginAnimation1DObject` or `ChromaSDKPluginAnimation2DObject`.
Use the variable `SET` so the `animation asset` can be selected from the dropdown.

![image_37](images/image_37.png)

Use `Load` on the `animation` before it can be used.
Use `IsLoaded` to check if the `animation` has been loaded.

![image_40](images/image_40.png)

After the `ChromaSDK Animation` has been loaded and set in a variable, it can be played.

![image_38](images/image_38.png)

A playing `ChromaSDK Animation` can be stopped.

![image_39](images/image_39.png)

Use `Unload` to unload the animation.
`ChromaSDKPluginBPLibrary::UnInit` will automatically unload animations.

![image_41](images/image_41.png)

<a name="blueprint-objects"></a>
## Blueprint objects

### FChromaSDKEffectResult

`FChromaSDKEffectResult` holds a `ChromaSDK` result and an `effect identifier`.

![image_12](images/image_12.png)

```c++
struct FChromaSDKEffectResult
{
	UPROPERTY(BlueprintReadOnly)
	int Result;

	UPROPERTY(BlueprintReadOnly)
	FChromaSDKGuid EffectId;
}
```

### EChromaSDKDeviceEnum

`EChromaSDKDeviceEnum` is a list of all ChromaSDK devices.

![image_13](images/image_13.png)

```c++
enum class EChromaSDKDeviceEnum : uint8
{
	DE_ChromaLink           UMETA(DisplayName = "ChromaLink"),
	DE_Headset		UMETA(DisplayName = "Headset"),
	DE_Keyboard		UMETA(DisplayName = "Keyboard"),
	DE_Keypad		UMETA(DisplayName = "Keypad"),
	DE_Mouse		UMETA(DisplayName = "Mouse"),
	DE_Mousepad		UMETA(DisplayName = "Mousepad")
};
```

### EChromaSDKDevice1DEnum

`EChromaSDKDevice1DEnum` is a device that uses a one-dimensional array for custom effects.

![image_14](images/image_14.png)

```c++
enum class EChromaSDKDevice1DEnum : uint8
{
	DE_ChromaLink	        UMETA(DisplayName = "ChromaLink"),
	DE_Headset		UMETA(DisplayName = "Headset"),
	DE_Mousepad		UMETA(DisplayName = "Mousepad")
};
```

### EChromaSDKDevice2DEnum

`EChromaSDKDevice2DEnum` is a device that uses a two-dimensional array for custom effects.

![image_15](images/image_15.png)

```c++
enum class EChromaSDKDevice2DEnum : uint8
{
	DE_Keyboard		UMETA(DisplayName = "Keyboard"),
	DE_Keypad		UMETA(DisplayName = "Keypad"),
	DE_Mouse		UMETA(DisplayName = "Mouse"),
};
```

### FChromaSDKColors

`FChromaSDKColors` holds an array of colors.

![image_16](images/image_16.png)

```c++
struct FChromaSDKColors
{
	UPROPERTY(BlueprintReadOnly)
	TArray<FLinearColor> Colors;
};
```

### EChromaSDKKeyboardKey

`EChromaSDKKeyboardKey` represents a key on the keyboard.

![image_30](images/image_30.png)

```c++
enum class EChromaSDKKeyboardKey : uint8
{
	KK_ESC                           UMETA(DisplayName = "Esc (VK_ESCAPE)"),
	KK_F1                            UMETA(DisplayName = "F1 (VK_F1)"),
	...
	KK_KOR_7                         UMETA(DisplayName = "(0xFF)"),
	KK_INVALID                       UMETA(DisplayName = "Invalid keys.")
};
```

### EChromaSDKMouseLed

`EChromaSDKMouseLed` represents an led on the mouse

![image_31](images/image_31.png)

```c++
enum class EChromaSDKMouseLed : uint8
{
	ML_SCROLLWHEEL		UMETA(DisplayName = "Scroll Wheel LED"),
	ML_LOGO			UMETA(DisplayName = "Logo LED"),
	...
	ML_RIGHT_SIDE6		UMETA(DisplayName = "Right LED 6"),
	ML_RIGHT_SIDE7		UMETA(DisplayName = "Right LED 7")
};
```

<a name="blueprint-functions"></a>
## Blueprint functions

### ChromaSDKPlugin BP Library

1. bool UChromaSDKPluginBPLibrary::IsPlatformWindows()
    * Returns true if the platform is windows. Although this call works on all platforms, all the remaining BP methods only work on the Windows platform
    
2. int UChromaSDKPluginBPLibrary::ChromaSDKInit()
    * Returns the result of calling ChromaSDK::Init(). Initializes the ChromaSDK.
    
3. int UChromaSDKPluginBPLibrary::ChromaSDKUnInit()
    * Returns the result of calling ChromaSDK::UnInit. Uninitializes the ChromaSDK.

4. FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectNone(const EChromaSDKDeviceEnum&amp; device)
    * Returns the FChromaSDKEffectResult after creating a `CHROMA_NONE` effect given the device. This effect clears the lighting effect for a device.

5. FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectStatic(const EChromaSDKDeviceEnum&amp; device, const FLinearColor&amp; color)
    * Returns the FChromaSDKEffectResult after creating a `CHROMA_STATIC` effect given the device and color. This effect sets the lighting effect to a static color for a device.

6. FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom1D(const EChromaSDKDevice1DEnum&amp; device, const TArray&lt;FLinearColor&gt;&amp; colors)
    * Returns the FChromaSDKEffectResult after creating a `CHROMA_CUSTOM` effect given the device and one-dimensional color array. This effect sets the lighting effect with an array of colors for a device.

7. FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom2D(const EChromaSDKDevice2DEnum&amp; device, const TArray&lt;FChromaSDKColors&gt;&amp; colors)
    * Returns the FChromaSDKEffectResult after creating a (`CHROMA_CUSTOM` or `CHROMA_CUSTOM2`) effect given the device and two-dimensional color array. This effect sets the lighting effect with an array of colors for a device.

8. int UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(const FChromaSDKGuid&amp; effectId)
    * Returns the result of calling ChromaSDK::SetEffect given the effect identifier. Activates the given effect.
    
9. int UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(const FChromaSDKGuid&amp; effectId)
    * Returns the result of calling ChromaSDK::DeleteEffect given the effect identifier. Deletes the given effect.

10. TArray&lt;FLinearColor&gt; CreateColors1D(const EChromaSDKDevice1DEnum&amp; device)
    * Returns a blank one-dimensional color array given the device.

11. TArray&lt;FChromaSDKColors&gt; CreateColors2D(const EChromaSDKDevice2DEnum&amp; device)
    * Returns a blank two-dimensional color array given the device.

12. TArray&lt;FLinearColor&gt; CreateRandomColors1D(const EChromaSDKDevice1DEnum&amp; device)
    * Returns a one-dimensional random color array given the device.

13. TArray&lt;FChromaSDKColors&gt; CreateRandomColors2D(const EChromaSDKDevice2DEnum&amp; device)
    * Returns a two-dimensional random color array given the device.

14. const TArray&lt;FChromaSDKColors&gt;&amp; SetKeyboardKeyColor(const EChromaSDKKeyboardKey&amp; key, const FLinearColor&amp; color, TArray&lt;FChromaSDKColors&gt;&amp; colors)
    * Returns a modified two-dimensional color array with the key color changed.

15. const TArray&lt;FChromaSDKColors&gt;&amp; SetMouseLedColor(const EChromaSDKMouseLed&amp; led, const FLinearColor&amp; color, TArray&lt;FChromaSDKColors&gt;&amp; colors)
    * Returns a modified two-dimensional color array with the led color changed.

<a name="examples"></a>
## Examples

### Example Level

The Example Level displays a UI `Blueprint Widget` with buttons that have `onclick events` that use the `ChromaSDKPlugin`.

1 Open the level in `Content/Levels/ExampleLevel`

![image_1](images/image_1.png)

2 Open the UI `Blueprint Widget` in `Content/Blueprints/ExampleWidget_BP`

![image_2](images/image_2.png)

3 Open the `Level Blueprint` with the `Blueprints->Open Level Blueprint` menu item

![image_3](images/image_3.png)

4 The main features of the example level will be labelled with comment blocks

![image_4](images/image_4.png)

The example begins at `Event BeginPlay` which is executed on launch.
The `Setup UI` custom event loads the `Blueprint Widget`.
If the `Windows` platform is detected, the `ChromaSDKInit` custom event is called.

![image_7](images/image_7.png)

The `SetupUI` custom event loads the UI `Blueprint Widget` and saves a `widget` variable to be referenced by later UI controls.
The `Blueprint Widget` is added to the viewport and then the `SetupUIButtons` custom event is called.

![image_8](images/image_8.png)

The `SetupUIButtons` custom event uses the `widget` variable to access the named buttons to bind `onclicked` events.
When a button is clicked the `Event` delegate is invoked.
Since the `Event` delegate doesn't take parameters, an unnamed custom event is used to prepare the button parameters and invoke the `SetAllDeviceStaticColor` custom event.
`FLinearColor` is used to mimize the number of steps when passing colors to events and the SDK.
This same setup is used for all the static color buttons.

![image_9](images/image_9.png)

The `SetupUIButtons` custom event calls other custom events to setup the clear button, the random button, and the animation buttons.

![image_10](images/image_10.png)

The example ends at `Event EndPlay` which executes when the game exits or `ESC` is pressed in the editor.
If the `Windows` platform is detected, the `ChromaSDKUninit` custom event is called.

![image_11](images/image_11.png)

<a name="getting-started"></a>
## Getting Started

**Note: `ChromaSDKPlugin` calls should be made on the Windows platform.**

`DebugPrintResult` is an example custom event to check for result codes and print a message. The custom event takes parameters to include in the print message. A result code of zero is commonly used in the `API` to mean success. A success message is printed if detected. If an error message is printed if the result code is not zero.

![image_19](images/image_19.png)

### Init

**int UChromaSDKPluginBPLibrary::ChromaSDKInit()**

`Init` returns the result of calling ChromaSDK::Init(). Initializes the ChromaSDK. A result of zero indicates success. Upon success, effects can be created, set, and deleted.
    
![image_17](images/image_17.png)

### UnInit

**int UChromaSDKPluginBPLibrary::ChromaSDKUnInit()**

`UnInit` returns the result of calling ChromaSDK::UnInit. Uninitializes the ChromaSDK. A result of zero indicates success.

![image_18](images/image_18.png)

### CreateEffectNone

**FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectNone(const EChromaSDKDeviceEnum&amp; device)**

`CreateEffectNone` returns the `FChromaSDKEffectResult` after creating a `CHROMA_NONE` effect given the device. This effect clears the lighting effect for a device. A result of zero indicates success.

![image_20](images/image_20.png)

After effects are created, this example shows expanding the result to check the result for success. The `effect identifier` can be used to activate the effect with `SetEffect` or delete the effect with `DeleteEffect`.

![image_21](images/image_21.png)

After an effect has been set, check the result for success. Either success or failure, the `effect identifier` can be deleted with `DeleteEffect`.

![image_22](images/image_22.png)

After an effect has been deleted, check the result for success. After effects have been created they can be used and then deleted or kept around to be reused. Be sure to delete effect identifiers before exiting for proper cleanup.

![image_23](images/image_23.png)

### CreateEffectStatic

**FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectStatic(const EChromaSDKDeviceEnum&amp; device, const FLinearColor&amp; color)**

`CreateEffectStatic` returns the `FChromaSDKEffectResult` after creating a `CHROMA_STATIC` effect given the device and color. This effect sets the lighting effect to a static color for a device.

![image_24](images/image_24.png)

### CreateEffectCustom1D

**FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom1D(const EChromaSDKDevice1DEnum&amp; device, const TArray&lt;FLinearColor&gt;&amp; colors)**

`CreateEffectCustom1D` returns the FChromaSDKEffectResult after creating a `CHROMA_CUSTOM` effect given the device and one-dimensional color array. This effect sets the lighting effect with an array of colors for a device.

![image_25](images/image_25.png)

### CreateEffectCustom2D

**FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom2D(const EChromaSDKDevice2DEnum&amp; device, const TArray&lt;FChromaSDKColors&gt;&amp; colors)**

`CreateEffectCustom2D` returns the FChromaSDKEffectResult after creating a (`CHROMA_CUSTOM` or `CHROMA_CUSTOM2`) effect given the device and two-dimensional color array. This effect sets the lighting effect with an array of colors for a device.

![image_26](images/image_26.png)

### SetEffect

**int UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(const FChromaSDKGuid&amp; effectId)**

`SetEffect` returns the result of calling ChromaSDK::SetEffect given the effect identifier. Activates the given effect.

![image_21](images/image_21.png)
    
### DeleteEffect

**int UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(const FChromaSDKGuid&amp; effectId)**

`DeleteEffect` returns the result of calling ChromaSDK::DeleteEffect given the effect identifier. Deletes the given effect.

![image_22](images/image_22.png)

### CreateColors1D

**TArray&lt;FLinearColor&gt; CreateColors1D(const EChromaSDKDevice1DEnum&amp; device)**

`CreateColors1D` returns a blank one-dimensional color array given the device.

![image_28](images/image_28.png)

### CreateColors2D

**TArray&lt;FChromaSDKColors&gt; CreateColors2D(const EChromaSDKDevice2DEnum&amp; device)**

`CreateColors2D` returns a blank two-dimensional color array given the device.

![image_29](images/image_29.png)

### CreateRandomColors1D

**TArray&lt;FLinearColor&gt; CreateRandomColors1D(const EChromaSDKDevice1DEnum&amp; device)**

`CreateRandomColors1D` returns a one-dimensional random color array given the device.

![image_25](images/image_25.png)
    
### CreateRandomColors2D

**TArray&lt;FChromaSDKColors&gt; CreateRandomColors2D(const EChromaSDKDevice2DEnum&amp; device)**

`CreateRandomColors2D` returns a two-dimensional random color array given the device.

![image_26](images/image_26.png)

### SetKeyboardKeyColor

**const TArray&lt;FChromaSDKColors&gt;&amp; SetKeyboardKeyColor(const EChromaSDKKeyboardKey&amp; key, const FLinearColor&amp; color, TArray&lt;FChromaSDKColors&gt;&amp; colors)**

`SetKeyboardKeyColor` returns a modified two-dimensional color array with the key color changed.

![image_27](images/image_27.png)

### SetMouseLedColor

**const TArray&lt;FChromaSDKColors&gt;&amp; SetMouseLedColor(const EChromaSDKMouseLed&amp; led, const FLinearColor&amp; color, TArray&lt;FChromaSDKColors&gt;&amp; colors)**

`MouseLedColor` returns a modified two-dimensional color array with the led color changed.

![image_32](images/image_32.png)
