# Substrait Text Plan Language Reference

## Types
Types are at the core of Substrait plans.  Functions consume data of a particular type and output data in the 
same or a different type.  Nullable types in Substrait are represented by appending a question mark to the type.

It's worth noting that there are no unsigned versions of numeric types in Substrait but they can be implemented with
 user-defined types.

| Type                  | Description                                                              |
|-----------------------|--------------------------------------------------------------------------|
| bool                  | boolean                                                                  |
|                       |                                                                          |
| i8                    | one byte signed integer                                                  |
| i16                   | two byte signed integer                                                  |
| i32                   | four byte signed integer                                                 |
| i64                   | eight byte signed integer                                                |
|                       |                                                                          |
| fp32                  | four byte signed float                                                   |
| fp64                  | eight byte signed float                                                  |
|                       |                                                                          |
| string                | string                                                                   |
|                       |                                                                          |
| timestamp             | timestamp without a timezone                                             |
| date                  | date (day resolution)                                                    |
| time                  | time within a day (microsecond resolution)                               |
|                       |                                                                          |
| interval_year         | duration specified in years and months                                   |
| interval_day          | duration specified in days, hours, minutes, seconds, and microseconds    |
| timestamp_tz          | timestamp with a specified timezone                                      |
| uuid                  | 128-bit UUID                                                             |
|                       |                                                                          |
| varchar\<L>           | a variable sized character sequence of up to length L                    |
| fixedchar\<L>         | a character sequence of fixed length L                                   |
| fixedbinary\<L>       | a fixed length binary sequence of length L                               |
| decimal\<P,S>         | a decimal value with precision P and scale S                             |
|                       |                                                                          |
| struct\<T1,T2,...,TN> | a structure consisting of N specified types (may include nested structs) |  
| list\<T>              | a list of type T where T can be any type (including a list)              | 
| map\<K,V>             | a map with keys of type K and values of type V                           |

Note that user defined types are not yet supported in text plans (but will be at a future date).

#### Example Types

|                                        | Explanation                                                         |
|----------------------------------------|---------------------------------------------------------------------|
| i64\?                                  | nullable int64                                                      |
| varchar\<5>                            | a string with up to five characters                                 |
| struct\<string,i8,i32?,timestamp_tz>   | a structure with a string, two ints, one nullable, and a timestamp  |
| struct\<string,struct\<string,string>> | a structure containing a string and a substructure with two strings |

## Literals

Types are important in Substrait plans as the choice of type controls which implementation of a given function should
be used.  In the text representation of plans we label literals with their types to make it clear which function should
be invoked within the plan. The type of a literal is appended to the literal behind an underscore (_).

### Integer Literals

There are four different integer widths.  While other languages provide for binary, octal, and hexadecimal
representations of their integer literals (as well as spacing separators) human readability of literals isn't a current
requirement of the language and so those representations are not included in the text form of plans.

#### Example Integer Literals
```
123_i8
0_i16
123_i32
1234_i64
```

### Floating Point Literals

Floating point literals are represented with by a decimal number followed by an nullable exponent.  Positive exponents
must be prefixed by a plus sign.

#### Example Floating Point Literals

```
-1_fp32
0.3_fp64
2.3+2_fp32
1.99E-13_fp64
```

### Decimal Literals

Decimal literals are similar to that of floating point literals however are represented by a real number with an
exponent which is specified as part of the type.  The type includes the precision and the scale (number of base 10
digits to translate the result by). 

#### Example Decimal Literals

```
-123_decimal<3,0>  // -123
42_decimal<5,-4>  // 420000
```

### String Literals

Strings with no size limit may be specified without a type specifier for brevity.  Substrait text plans are UTF-8
encoded throughout so any characters contained in strings must also be valid UTF-8.  For byte content that is not in
UTF-8 use a series of byte escape sequences instead.

The syntax for raw strings is any number of backticks followed by any unescaped characters followed by a similar number
of backticks.

| Escape Sequence | Description                         |
|-----------------|-------------------------------------|
| \n              | linefeed (ASCII code 10)            |
| \r              | carriage return (ASCII code 13)     |
| \t              | tab (ASCII code 8)                  |
| \\              | backslash                           |
| \'              | single quote                        |
| \"              | double quote                        |
| \xNN            | two byte hexadecimal character code |
| \u{NNNNNNN}     | unicode code sequence               |

#### Example String Literals

```
"simple text"
"two\nlines with \"escapes\""_varchar<80>
"abcde"_fixedchar<5>
"bytes: \xA9\x72"
"unicode char: \u{023B}"
`raw string with a Windows path: C:\file.txt`
``string with a backtick (`) in it``
```

### Binary Literals

Binary literals are represented as strings containing hexadecimal digits (no escapes required).  Fixed binary literals
are measured in bytes and thus have two hexadecimal characters per required byte.  A UUID is effectively a
fixedbinary\<16> that ignores any provided dashes in its representation. 

#### Example Binary Literals

```
"0123456789abcde"_binary
"1234"_fixedbinary<2>
"ddb287e8-7d4c-4fad-b2e7-07428be043e5"_uuid
```

### Time Literals

There are two kinds of time literals:  literals that represent particular points in time and literals that represent
durations.  The timestamps represent dates from the beginning of the year 1000 to the end of the year 9999.  The 
interval_year duration has its precision in months and the interval_day duration has its precision in microseconds.  The
format of timestamps are ISO-8601.

To make the structured data easier to read the pseudo types ```year```, ```month```, ```day```, ```second```, and
```microsecond``` (plural or singular) are suggested for readability.

#### Example Time Literals

```
"1000-01-01 00:00:00.000000"_timestamp
"1000-01-01 00:00:00.000000 UTC"_timestamp_tz
"2020-12-20"_date
"13:21"_time
"13:21:12.012345"_time

{5_years, 1_month}_interval_year
{5_year, 1_months}_interval_year
{4_days, 1_second, 13_microseconds}_interval_day
```

### Structured Literals

Structure literals use the brace syntax.  Types are not necessary within the value section as the types should be 
homogeneous if the structure is well-formed. 

|                                                            | Description                                   |
|------------------------------------------------------------|-----------------------------------------------|
| {}_list\<string>                                           | empty list of string                          |
| {}_list\<string?>                                          | empty list of nullable strings                |
| {null, "a", "b"}_list\<string?>                            | list of nullable strings                      |
| {}_map\<int, string>                                       | empty map with integer keys and string values |
| {42 : "life", 32 : "everything"}_map\<int, string>         | map with integer keys and string values       |
| {"a", {"b", "c"}}_struct\<string, struct\<string, string>> | structure with nested substructure            |

#### Example Structured Literals
```
{"a", "b", "c"}_list<string>
```

### Boolean Literals

The values for boolean literals are distinct from all other literals so appending the type is not required.

#### Example Boolean Literals
```
true
false
true_bool
false_bool
```

### Null

Null also has a distinct form.  Types are still necessary to inform which function is to be used to process the literal.
All places where null can be used must be nullable and so for null literals it is redundant to say that the value is
null and it is from a nullable type.  For brevity the question mark may be eliminated.

Nulls can represent either an unknown value or the lack of a value depending on the semantics of the operation.  (A sum
function using null as unknown would result in unknown, but a sum operation treating null as not present would result in
the total of the non-null elements.)

#### Example Null Literals
```
null_i32  // Exactly the same as null_i32?
null_i32?
null_struct<string,struct<string,string>>
```
