# yaul XML Features #
The yaul XML parser will parse all valid XML files. The following is a list of elements described in yaul XML files.

The root element for all XML documents is the `<yaul>` tag. There can only be one `<yaul>` element in a document. It does not support any attributes.

## Global Attributes ##
Global attributes can be applied to any element.

### `class` - Not implemented yet  ###
A space-separated list of classes of the element allowing CSS access via selectors.

_Must start with a letter and contain no whitespace; spaces indicate multiple classes._

### `dir` - Not implemented yet  ###
An enumerated attribute that indicates the directionality of an element's text.

Enumeration values:
* `ltr` - _left to right_
* `rtl` - _right to left_
* `auto` (default) - directionality is determined by the element's contents

### `hidden` - Not implemented yet  ###
An enumerated attribute to indicate to not render an element.

Enumeration values:
* `true` - element is not visible and does not affect layout. Equivalent to deleting the element.
* `invisible` - element is not visible and does affect layout. Equivalent to replacing with a transparent element of identical size.
* `false` (default) - element is visible and does affect layout.

### `id` - Not implemented yet  ###
A unique identifier to reference elements throughout the application.

_Must start with a letter and contain no whitespace._

### `lang` - Not implemented yet  ###
A BCP47 compliant language tag indicating the language of an element's text.

_Must be a BCP47 compliant language tag. default: `unknown`_

### `style` - Not implemented yet  ###
CSS declarations applied directly to the element.

---

## `<window>` - Not implemented yet ##
The `<window>` tag represents a window on the user's screen. A `<window>` must be a direct child of the `<yaul>` tag.

Along with the global attributes, `<window>` has the following attributes:

### `title` ###
Title of the window which is displayed in the menu bar and desktop manager.

---

## `<string>` - Not implemented yet ##
The `<string>` tag represents a single piece of text on the user's screen.

Along with the global attributes, `<string>` has the following attributes:

### `text` ###
Text of the string