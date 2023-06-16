

UMG
---
UMG初始化的时候调用LoadURL方法加载页面。
![](Images\1.png)
如果需要跳转页面，要重载或绑定OnBeforePopup事件，拿到URL直接加载即可跳转
![](Images\2.png)
Slate
SWebBrowser
Slate类比较重要的函数就是Construct
![](Images\3.png)
可以看到SWebBrowser中，承载Web功能的主要slate类的功能是SWebBrowserView这个类，ue在不同的操作系统上配置了不同的浏览器内核，Windows平台使用了CEF3，Chromium的开源项目.
这里有比较重要的几个事件和方法：
OnLoadCompleted：网页加载完成回调，这个回调是当页面加载好后触发
OnBeforePopup：页面跳转前会触发，通常用于处理页面跳转
OnCreateWindow：OnBeforePopup返回false后触发

JS注入
---
（1）JavaScript调用UE：利用BindObject方法，可以在JS环境中注入UE对象
```cpp
void BindUObject(const FString& Name, UObject* Object, bool bIsPermanent = true);
```

![](Images\4.png)

- JS环境中的调用方法为window.ue.Name.FunctionName.
- FunctionName为对象中使用UFUCNTION宏标记的函数，Name就是绑定是传入的name
- 通过使用 BindUObject，可以将一个 UObject 实例（通常是一个 C++ 或蓝图类的实例）绑定到 WebBrowser 组件，并在网页中调用该对象的函数或访问其属性。

下面是一个简单的示例，展示如何在 UE4 中使用 BindUObject：
```cpp
// 在 C++ 类中声明一个 UFUNCTION，该函数将绑定到 WebBrowser 组件
UFUNCTION(BlueprintCallable, Category = "MyWebObject")
void MyFunction(FString Message);
```
// 在 C++ 类的 .cpp 文件中实现该函数
```cpp
void UMyObject::MyFunction(FString Message)
{
    // 在这里编写函数逻辑
    UE_LOG(LogTemp, Warning, TEXT("WebBrowser Message: %s"), *Message);
}
```
然后，在你的蓝图或其他地方，你可以将该对象的实例与 WebBrowser 组件进行绑定：
```cpp
// 获取 WebBrowser 组件的引用
UWebBrowser* MyWebBrowser = // 获取 WebBrowser 组件的引用

// 将 UObject 实例绑定到 WebBrowser 组件
MyWebBrowser->BindUObject("myobject", MyObjectInstance);
```
在网页中，你可以使用 JavaScript 代码来调用绑定的 UObject 实例的函数，如下所示：
```cpp
// 在网页中调用绑定的函数
ue.myobject.myfunction("Hello from the web page!");
```

这样，当网页中调用 MyFunction 函数时，UE 中的 MyObject 实例将执行相应的逻辑。
（2）UE调用JavaScript：使用ExecuteJavascript运行js脚本
![](Images\5.png)
