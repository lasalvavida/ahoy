# Ahoy!
C++11 Argument Parser Library

[![Build Status](https://travis-ci.org/lasalvavida/ahoy.svg)](https://travis-ci.org/lasalvavida/ahoy)
[![Build Status](https://ci.appveyor.com/api/projects/status/an23o90rkak5jh8i/branch/master)](https://ci.appveyor.com/project/lasalvavida/ahoy/history)

## About
Ahoy! is a C++11 argument parser designed for ease of use.

You define your inputs and destinations, and Ahoy! will automatically push the parsed arguments to the right places.
Ahoy! also generates a nice help dialog using this information for command line interfaces.

## Releases

Compiled binaries for Windows and Linux can be found under [releases](https://github.com/lasalvavida/ahoy/releases). It is recommended to use the last versioned release

A live build of the current master branch is available as [latest](https://github.com/lasalvavida/ahoy/releases/tag/latest).
These binaries are updated whenever master changes, the build succeeds, and the tests pass. These binaries are bleeding-edge and are not guaranteed to be stable.

## Compile from source

1. Clone repository

  ```bash
  git clone --recursive https://github.com/lasalvavida/ahoy.git
  ```
2. Compile

  ```bash
  cd ahoy
  mkdir build
  cd build
  cmake .. #-Dtest=ON
  # Linux
  make
  # Windows
  ## Open the generated ahoy.sln in Visual Studio and build
  ```

3. Run tests

  ```bash
  ahoy-test[.exe]
  ```

## Usage
### Example
#### C++
```c++
int main(const int argc, const char** argv) {
    bool flag;
    int number;

    Parser* parser = new Parser();
    parser->name("Ahoy! Basic Example")
        ->usage("main [options]")
        ->example("main -f")
        ->example("main -n 10 -f");

    parser->define("f", &flag)
        ->alias("flag")
        ->description("A boolean flag")
        ->require();

    parser->define("n", &number)
        ->alias("number")
        ->description("A number")
        ->defaults(5);

    bool result = parser->parse(argc, argv);
    delete parser;

    if (!result) {
        return 1;
    }

    printf("%s\n", flag ? "true" : "false");
    printf("%d\n", number);
    return 0;
};
```
#### I/O
```
<< main -h
>> Ahoy! Basic Example
   Usage:
     main [options]

   Options:
     -f, --flag   A boolean flag   [required] [bool]
     -n, --number A number         [int] [default: 5]

   Examples:
     main -f
     main -n 10 -f

<< main -f
>> true
   5

<< main -n 10 --flag
>> true
   10
```

### Reference
Ahoy! contains definitions for the following types:
* *bool*
* *double*
* *int*
* *string*
* *vector* (of any of the above types)

All property functions are chainable, that is they return *this*, so you can do *object->func()->otherfunc();*.
#### Parser
* *define(string, T*)* - Define a new *TypedOption* of type *T* with a string key, returns *TypedOption* for chaining.
##### Property Functions
* *example(string)* - Add a *string* showing an example using the program.
* *help()* - Get a *string* containing the program help message.
* *strict(bool)* - Throw error if an unknown command line flag is passed in.
* *quiet(bool)* - Enable/disable printing to command line.
* *usage(string)* - Add a *string* showing how to use the program.

#### TypedOption
##### Property Functions
* *alias(string)* - Define an alias for this option.
* *count(int)* - For *vector* types, this defines a maximum number of arguments to consume.

    *vector* types consume all arguments after them until they hit another option flag, or they consume *count* arguments

    For example,

    `example -vecInt 1 2 3 4 -otherInt 5`

    would consume `1`, `2`, `3`, and `4` into the vector option, but if count is set to be 2, it would consume only `1` and `2`.

* *defaults(T)* - Define a default value for this option.
* *description(string)* - Define a description for this option.
* *index(int)* - Assign an index to this option; when there are additional arguments not belonging to an option, index determines if those loose arguments should be used to fill this option; lower indices have a higher priority.

    For example, if there is a program that takes two arguments, an input and an output file path, you could define the index for those options to be 0 and 1 respectively. In this way, you can call the program normally as

    `example --input inputPath --output outputPath`

    or just

    `example inputPath outputPath`

     By doing this, you can still *require* `input` and `output` even when they are filled from loose arguments.

* *require()* - Throw error if this option is not present

### Advanced
#### Extensibility for Custom Types
Ahoy! uses templates under the hood, so it's pretty easy to add support for other types. You just need to define two functions in the `ahoy` namespace:

```c++
namespace ahoy {
  bool TypedOption<Type>::parse(const int argc, const char** argv, int* positionPtr, string* parseError);
  string TypedOption<Type>::help();
}
```

*parse* defines how this option should handle the input. *argc* and *argv* are the raw command line parameters and *positionPtr* is a pointer to the current position in *argc*. The function is expected to update *positionPtr* if it consumes additional arguments. It should return *false* if there is an error, and update *parseError* with a description of what went wrong if possible.

*help* defines what this option should put on the command-line. A recommended implementation would be something like:
```c++
template<>
string TypedOption<Type>::help() {
	return Option::help() + " [Type]" + (hasDefault ? (" [default: " + to_string(defaultValue) + "]") : "");
}
```

If those two functions are defined for your type, the compiler will do the rest of the work for you.

## Contributing
Contributions are welcome. Template definitions for native and STL types are also welcome, but anything outside of that scope should be implemented just in your project or as a separate extension library.

Please run the tests locally before opening a pull request.

## License
Ahoy! is published under the MIT license. See [LICENSE](LICENSE) for more information.
