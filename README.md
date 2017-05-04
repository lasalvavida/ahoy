# Ahoy!
C++11 Argument Parser

## About
Ahoy! is a C++11 argument parser designed for ease of use.

You define your inputs and destinations, and Ahoy! will automatically push the parsed arguments to the right places.
Ahoy! also generates a pretty help dialog using this information for nice command line interfaces.

## Examples
### Basic
#### C++
```c++
int main(const int argc, const char** argv) {
    bool flag;
    int number;

    Parser* parser = new Parser();
    parser->name("Ahoy! Basic Example")->usage("main [options]")->example("main -f")->example("main -n 10 -f");
    parser->define("f", &flag)->alias("flag")->description("A boolean flag")->require();
    parser->define("n", &number)->alias("number")->description("A number")->default(5);

    if (!parser->parse(argc, argv)) {
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
