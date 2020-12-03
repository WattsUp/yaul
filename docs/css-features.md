# yaul CSS Features #
The yaul CSS parser will parse valid CSS files. It is not fully compliant, the following lists which features are supported.
## Selectors ##
### Basic Selectors ###
| Feature | Description | Status |
|---|---|---|
| `*` | Universal selector | Not implemented yet |
| _`elementname`_ | Type selector | Not implemented yet |
| _`.classname`_ | Class selector | Not implemented yet |
| _`#idname`_ | ID selector | Not implemented yet |
| _`[attr=value]`_ | Attribute selector | Not implemented yet |

### Combinators ###
| Feature | Description | Status |
|---|---|---|
| **_`A `_`+`_` B`_** | Adjacent sibling selector | Not implemented yet |
| **_`A `_`~`_` B`_** | General sibling selector | Not implemented yet |
| **_`A `_`>`_` B`_** | Child combinator | Not implemented yet |
| **_`A B`_** | Descendent combinator | Not implemented yet |
| **_`A `_`||`_` B`_** | Column combinator | Not implemented yet |

### Pseudo-classes ###
| Feature | Description | Status |
|---|---|---|
<!-- | `:dir()` | Element has left to right or right to left directionality | Not implemented yet |
| `:lang()` | Element has specific language | Not implemented yet |
| `:any-link` | Element is a hyperlink | Not implemented yet |
| `:link` | Element is a hyperlink not yet visited | Not implemented yet |
| `:visited` | Element is a hyperlink already visited | Not implemented yet |
| `:local-link` | Element is a hyperlink to current URL | Not supported |
| `:target` | Element is the target of the current URL | Not supported |
| `:target-within` | Element is the target of the current URL | Not supported |
| `:scope` | Element is a designated reference element | Not supported |
| `:current` | Element is currently presented in a time-dimensional canvas | Not supported |
| `:past` | Element is in the past in a time-dimensional canvas | Not supported |
| `:future` | Element is in the future in a time-dimensional canvas | Not supported |
| `:active` | Element is activated by the user | Not implemented yet |
| `:hover` | Element is under the cursor | Not implemented yet |
| `:focus` | Element has user input focus | Not implemented yet |
| `:focus-within` | Element has user input focus or an element that has user input focus | Not implemented yet |
| `:focus-visible` | Element has user input focus and an indicator should be drawn for that element | Not implemented yet |
| `:enabled, :disabled` | User interface element is enabled or disabled | Not implemented yet |
| `:read-write, :read-only` | User interface element is alterable or not | Not implemented yet |
| `:placeholder-shown` | Input control currently showing placeholder text | Not implemented yet |
| `:default` | User interface element is the default in a group of related choices | Not implemented yet |
| `:checked` | User interface element that is checked or toggled on | Not implemented yet |
| `:indeterminate` | User interface element that is in an indeterminate state | Not implemented yet |
| `:valid, :invalid` | User-input element meets, or doesn't, validation | Not implemented yet |
| `:in-range, :out-of-range` | User-input element whose value is in-range/out-of-range | Not implemented yet |
| `:required, :optional` | User-input element requires/does not require input | Not implemented yet |
| `:blank` | User-input element whose value is blank/empty | Not implemented yet |
| `:user-invalid` | User-altered user-input element with incorrect input | Not implemented yet |
| `:root` | Root of application, used for global declaration such as color pallette | Not implemented yet |
| `:empty` | Element with no children | Not implemented yet |
| `:nth-child()` | The n-th child element of its parent | Not implemented yet |
| `:nth-last-child()` | Same as `:nth-child()` except items are counted in reverse | Not implemented yet |
| `:first-child` | Element, first child of its parent | Not implemented yet |
| `:last-child` | Element, last child of its parent | Not implemented yet |
| `:only-child` | Element, only child of its parent | Not implemented yet |
| `:nth-of-type()` | Element, n-th sibling of its type | Not implemented yet |
| `:nth-last-of-type()` | Same as `:nth-of-type()` except items are counted in reverse | Not implemented yet |
| `:first-of-type` | Element, first sibling of its type | Not implemented yet |
| `:last-of-type` | Element, last sibling of its type | Not implemented yet |
| `:only-of-type` | Element, only sibling os its type | Not implemented yet |
| `:nth-col()` | Element belonging to n-th column in a grid/table | Not implemented yet |
| `:nth-last-col()` | Same as `:nth-col()` except items are counted in reverse | Not implemented yet |
| `:host` | Selects the shadow host of the shadow DOM | Not supported |
| `:is()` | Element is selected by one selector in a list | Not implemented yet |
| `:where()` | Same as `:is()` except it always has 0 specificity | Not implemented yet |
| `:not()` | Element does not match a list of selectors | Not implemented yet |
| `:first, :last, :left, :right` | `@page` selector | Not supported | -->

## Properties ##
| Feature | Description | Status |
|---|---|---|
