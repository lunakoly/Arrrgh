# About
Some handy code for handling cli arguments.

# General Things
Arrrgh supports _free parameters_, _options_ (`--option`) and _aliases_ to options (`-a`).
Options and aliases trigger _processor_ functions that are responsible of determining the proper value for them.
Processors (as well as option values) are not bound to strings.
Processor functions take the arguments stream as a parameter and may look
for any number of next values.

# Processors
## Single Argument Processor
Sets the next argument after `--option` as it's value discarding all previous values.
- Python: `single_argument_processor`
- C++: `single_argument_processor`

## Flag Processor
Assigns `True` to an `--option` when it's met. If used with the corresponding option registering function
`False` is automatically assigned as the default value.
- Python: `flag_processor`
- C++: `flag_processor`

## List Processor
Collects all values that follow immediately after `--option`. Expects the default value to be a list.
Default value is a list if the option is registered with the corresponding function
- Python: `list_processor`
- C++: `list_processor`

## Integer Processor
Sets the next argument after `--option` as it's value discarding all previous values
if it's an integer. Otherwise leaves the default value as is
- Python: `integer_processor`
- C++: `integer_processor`

# Python
## Example
Define options as:
```python
import arrrgh

arrrgh.add_option("source");
arrrgh.add_flag("flag");
arrrgh.add_list("tag");
arrrgh.add_integer("number", 10);

arrrgh.add_alias('s', "source");
arrrgh.add_alias('f', "flag");
arrrgh.add_alias('t', "tag");
arrrgh.add_alias('n', "number");

arrrgh.parse()
```

And access values via:
```python
for key, value in arrrgh.options.items():
  print("option: {} -> \"{}\"".format(key, value))

for key, value in arrrgh.aliases.items():
  print("alias: {} -> {}".format(key, value))

for it in arrrgh.parameters:
  print("parameter: {}".format(it))
```

## Docs
```python
def add_option(name, default_value='', processor=single_argument_processor)
```
- Registers a new option.

```python
def add_alias(alias_name, option_name)
```
- Registers a new alias. Requires an option to exist

```python
def add_flag(name)
```
- Registers a new boolean option. Value is `False` by default and `True` after the `--name` is met at least once.

```python
def add_list(name)
```
- Registers a new list option. Empty list by default. Collects all values that follow immidiately after `--name`

```python
def integer_processor(generator, old_value)
```

- Attempts to assign the next argument value to an option
if it's an integer. Otherwise leaves the default value as is

```python
def parse(stream=sys.argv)
```
- Analyses the arguments.

To declare a custom processor create a function taking (`generator`, `old_value`) parameters and returning some value.

# C++
## Example
Define options as:
```c++
#include "arrrgh.hpp"

arrrgh::add_option("source");
arrrgh::add_flag("flag");
arrrgh::add_list("tag");
arrrgh::add_integer("number", 10);

arrrgh::add_alias('s', "source");
arrrgh::add_alias('f', "flag");
arrrgh::add_alias('t', "tag");
arrrgh::add_alias('n', "number");

arrrgh::parse(argv, argv + argc);
```

And access values via:
```python
for (auto it : arrrgh::options<arrrgh::StringLike>) {
  std::cout << "option: " << it.first << " -> \"" << it.second << '\"' << std::endl;
}

for (auto it : arrrgh::options<int>) {
  std::cout << "option: " << it.first << " -> " << it.second << std::endl;
}

for (auto it : arrrgh::options<bool>) {
  std::cout << "option: " << it.first << " -> " << it.second << std::endl;
}

for (auto it : arrrgh::options<arrrgh::List>) {
  std::cout << "option: " << it.first << " -> [" << std::endl;

  for (auto that : it.second) {
    std::cout << "    " << that << std::endl;
  }

  std::cout << ']' << std::endl;
}

for (auto it : arrrgh::aliases) {
  std::cout << "alias: " << it.first << " -> " << it.second << std::endl;
}

for (auto it : arrrgh::parameters) {
  std::cout << "parameter: " << it << std::endl;
}
```

## Notes
C++ code introduces a bunch of type aliases. It's been made to make the code more readable during the development.
- `String` -> `std::string`
- `StringLike` -> `std::string_view`
- `Map<T, K>` -> `std::unordered_map<T, K>`
- `List` -> `std::vector<StringLike>`

## Docs
```c++
template <typename Value = StringLike>
void add_option(
    StringLike option_name,
    Value default_value = StringLike(),
    Processor processor = single_argument_processor
)
```
- Registers a new option.

```c++
void add_alias(
    char alias_name,
    StringLike option_name
)
```
- Registers a new alias. Requires an option to exist

```c++
void add_flag(StringLike option_name)
```
- Registers a new boolean option. Value is `False` by default and `True` after the `--name` is met at least once.

```c++
void add_list(StringLike option_name)
```
- Registers a new list option. Empty list by default. Collects all values that follow immidiately after `--name`

```c++
void add_integer(StringLike option_name, int default_value = 0)
```

- Registers a new integer option. If the new value can be converted to
a string via `std::from_chars`, converts it and assigns to `--name`

```c++
template <typename Iterator>
void parse(Iterator begin, Iterator end)
```
- Analyses the arguments.

```c++
using Processor = void (*)(StringLike, const Arguments &)
```
- Processor declaration. It accepts the name of the option as the first argument and a const reference the arguments
stream.

```c++
struct Arguments {
    virtual ~Arguments() {};

    /*
        Returns true if there other
        values left unread
    */
    virtual bool has_next() const = 0;

    /*
        Returns a StringLike of the
        next value of the stream.
        This helps to generalize
        const char *[] and std::string streams
    */
    virtual StringLike next() const = 0;
};
```
- Represents a sequence of `StringLike`. Actually just a bit
different form of passing iterators around
(in and out of the functions) in a way that
any inner function 'modifies the pointers
of the outer function'
