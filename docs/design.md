# yaul Design #
yaul strives to maintain a well defined separation from the application's _business logic_ and its _presentation_. This avoid complicated relationships between the information and the screen. In my opinion, C++ is a great language but writing GUIs in C++ is terrible. Instead, yaul uses XML-like files to describe components, layout, and hierarchy and a CSS-like files to style. As a result, the application's presentation is hot reloaded automatically with any file updates. This allows rapid development of the GUI without massive compile times. The interface can also be easily skinned for a different look-and-feel (light vs dark mode). For distribution, these files may be incorporated into the executable or into a compressed archive to avoid user modification or left where they are.

---

## Files ##
An application is programmed in C++. The GUI is described in XML files and styled in CSS files.

### C++ ###
From a C++ point of view, incorporating yaul is a few lines. The first step is to initialize the library and create a window. The following code snippet creates initializes yaul with a friendly application name (seen in Task Manager). It then creates a window with a unique ID. This id is used to refer to the same components across the three file types. After this execution a window will become visible to the user with the components and layout described in the other files. It runs its own thread to update and redraw the screen as well as manage any inputs.
```C++
auto app = ::yaul::Application(argc, argv, settings);
auto window = app.addWindow("unique window id");
```

The next step is to modify the window. The following code snippet adds an empty string to the window (since `"dialog"` is already added to the window via the XML file, this statement just gets a reference to it and makes sure it is empty). It then sets the text to "Hello world!" one letter at a time. As soon as the text object is mutated the GUI updates on the next redraw.
```C++
auto dialog = window.addText("dialog", "");

for (char c : "Hello world!") {
  dialog += c;
  ::std::this_thread::sleep_for(::std::chrono::seconds(1));
}
```
An application can exist without the use of XML/CSS files by explicitly describing elements in C++. The following code snippet has the same result as the XML and CSS examples.
```C++
auto window = ::yaul::addWindow("unique window id");
window.setTitle("Hello World Title");

window.addText(nullptr, "yaul says")
  .setColor(::yaul::color::BLUE)
  .setFont(::yaul::unit::dp(48.0));

auto dialog = window.addText("dialog", "")
  .setColor(::yaul::color::BLUE)
  .setFont(::yaul::unit::dp(48.0))
  .setTextDecoration(::yaul::textDecoration::UNDERLINE);

for (char c : "Hello world!") {
  dialog += c;
  ::std::this_thread::sleep_for(::std::chrono::seconds(1));
}
```
Notice how this method is more cumbersome. The key use of describing elements in C++ is when adding dynamic elements such as generating a playlist. Then one would use a loop to add top level elements whose contents are described by XML files normally.

### XML Files ###
XML files describe visual elements. Elements are declared and organized in this structure. For a list of elements, see [XML features list](https://github.com/WattsUp/yaul/blob/master/docs/xml-features.md). They can also be directly styled in this file via attributes. The following code snippet describes a window containing two strings. The first string is static and can not directly be referred to. The second string has an ID and is destined to be mutated by the application logic.
```XML
<?xml version="1.0" encoding="UTF-8"?>
<yaul>
<window id="unique window id" title="Hello World Title">
  <string text="yaul says:"/>
  <string id="dialog"/>
</window>
</yaul>
```

Creating custom elements is done by the use of curly braces to specify templates. The logic effectively does a string replacement between a template's attributes and its curly braces. The following code snippet describes a list of songs where each song has cover art, artist, title, and play button.
```XML
<?xml version="1.0" encoding="UTF-8"?>
<yaul>
<window id="unique window id" title="Hello World Title">
  <string text="yaul says:"/>
  <string id="dialog"/>
  <grid col="1" row="3">
    <song artwork="art/song1.png" title="Song 1" artist="Artist 1"/>
    <song artwork="art/song2.jpeg" title="Song 2" artist="Artist 2"/>
    <song artwork="art/song3.gif" title="Song 3" artist="Artist 3"/>
  </grid>
</window>
<song artwork="" title="" artist="">
  <image src="{artwork}"/>
  <string text="{title}"/>
  <string text="{artist}"/>
  <button text="&#9658;" id="play" value="{title}"/>
</song>
</yaul>
```

### CSS Files ###
CSS files style visual elements. Not all features of CSS are implemented, see [CSS features list](https://github.com/WattsUp/yaul/blob/master/docs/css-features.md) for a list of features. The code snippet below changes `big-blue` class elements to use a large blue font and the element with ID `dialog` to be underlined.
```XML
<string class="big-blue" text="yaul says:"/>
<string class="big-blue" id="dialog"/>
```
```CSS
.big-blue {
  color: blue;
  font-size: 48dp;
}

#dialog {
  text-decoration: underline;
}
```
---
## Front-end to Back-end Connection ##
yaul is designed for a Model-View-ViewModel (MVVM) architecture.
* The **Model** contains the application's business logic and its data. The Model remains decoupled enough such that it may be reused in different contexts such as a command-line interface.
* The **View** describes the visual elements the user sees and interacts with. This is done in XML and CSS files.
* The **ViewModel** links the Model and the View by providing binding targets. User interaction of the View is processed by event handlers in the ViewModel which may call Model methods or update the View directly. The Model updates values in the ViewModel which propagate changes to the View.

For each window, yaul has an `updateView` and `handleEvents` method. The former method reads data out of the ViewModel and uses the View to draw to the screen. The later method checks for any user interaction and handles those events via the ViewModel's bindings. The default case is to have a thread running these commands while events are handled asynchronously in children threads. yaul will wait for changes to the ViewModel and limit itself to a refresh rate before redrawing to reduce resource usage. For synchronous applications, the application can explicitly call these methods.

## Graphics Bindings ##
By default yaul will use the platform's default graphics API. DirectX for Windows, OpenGL for Linux, and Metal for Mac. yaul is also compatible with Vulkan. For platforms with multiple graphics APIs, a specific one may be explicitly specified. Hardware acceleration can be disabled to force CPU rendering.

yaul is able to be integrated into complex 3D applications such as CAD or games. The UI can overlay on top of a 3D scene and/or yaul can output to textures on 3D objects.

## Library Interface ##
### PImpl ###
To allow dynamic linking with backwards compatibility, the ABI remains stable though a PImpl (pointer to implementation) design. The user application creates a lightweight object that contains public functions that act on the private object found via the pointer to implementation.

The two classes `yaul::Application` and `yaul::Window` directly represent the private object. Meaning when those user application side object get destructed, the private object will also be destructed. All elements that get added to a Window are lightweight objects representing a weak pointer to the private element in the DOM tree. As such destruction of those objects do not trigger destruction of the real private objects. The destruction happens when the elements are removed from the DOM tree.

### Zero Dependencies ###
To meet zero dependencies of the library, all data on the interface side use primitive data types. Namely strings are passed as `const char*` and expect a UTF-8 encoding. In actuality, only CPU rendering meets the zero dependency goal. Using OpenGL, DirectX, Metal, or Vulkan require those libraries but they don't count.

### Exceptions ###
Any exceptional states will throw an exception. The exception contains an explanatory message suitable for logging. To avoid discrepancies between runtime on each side of the library boundary. Any library exceptions are caught and passed via string then rethrown.
