# ChromaSDKPlugin - Unreal Engine 4 (UE4) Blueprint library to control lighting for (Razer Chroma)

**Table of Contents**

* [Frameworks supported](#frameworks-supported)
* [Usage](#usage)
* [Blueprint objects](#blueprint-objects)
* [Blueprint functions](#blueprint-functions)
* [Examples](#examples)
* [Getting Started](#getting-started)

<a name="frameworks-supported"></a>
## Frameworks supported
- Windows ChromaSDK (32-bit)
- Windows ChromaSDK (64-bit)

<a name="usage"></a>
## Usage

1 Copy `ChromaSDKPlugin` to the `ProjectFolder\Plugins\ChromaSDKPlugin` folder

2 Open the `Plugins Browser tab` using the `Edit->Plugins` menu item

![image_5](/images/image_5.png)

3 Make sure the `ChromeSDKPlugin` is enabled

![image_6](/images/image_6.png)

<a name="blueprint-objects"></a>
## Blueprint objects

### FChromaSDKEffectResult

`FChromaSDKEffectResult` holds a `ChromaSDK` result and an `effect identifier`.

![image_12](/images/image_12.png)

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

![image_13](/images/image_13.png)

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

![image_14](/images/image_14.png)

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

![image_15](/images/image_15.png)

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

![image_16](/images/image_16.png)

```c++
struct FChromaSDKColors
{
	UPROPERTY(BlueprintReadOnly)
	TArray<FLinearColor> Colors;
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

6. FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom1D(const EChromaSDKDevice1DEnum&amp; device, const FChromaSDKColors&amp; colors)
    * Returns the FChromaSDKEffectResult after creating a `CHROMA_CUSTOM` effect given the device and one-dimensional color array. This effect sets the lighting effect with an array of colors for a device.

7. FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom2D(const EChromaSDKDevice2DEnum&amp; device, const TArray&lt;FChromaSDKColors&gt;&amp; colors)
    * Returns the FChromaSDKEffectResult after creating a (`CHROMA_CUSTOM` or `CHROMA_CUSTOM2`) effect given the device and two-dimensional color array. This effect sets the lighting effect with an array of colors for a device.

8. int UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(const FChromaSDKGuid&amp; effectId)
    * Returns the result of calling ChromaSDK::SetEffect given the effect identifier. Activates the given effect.
    
9. int UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(const FChromaSDKGuid&amp; effectId)
    * Returns the result of calling ChromaSDK::DeleteEffect given the effect identifier. Deletes the given effect.

10. FChromaSDKColors UChromaSDKPluginBPLibrary::CreateRandomColors1D(int elements)
    * Returns a one-dimensional color array of random colors given the number of elements to create.
    
11. TArray&lt;FChromaSDKColors&gt; UChromaSDKPluginBPLibrary::CreateRandomColors2D(int maxRows, int maxColumns)
    * Returns a two-dimensional color array of random colors given the number of rows and columns to create.

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
