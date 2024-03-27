# Tragex Documentation
Tragex is interpreted programming lanugage, that lets you play around with the program stack. It's relatively simple to learn, but that makes it not useful for actuall apps.

### Reference
* [`Basic Syntax`](#basic-syntax)
    * [`Declaring a new variable`](#declaring-a-new-variable)
    * [`Removing a variable`](#removing-a-variable)
    * [`Mathematical operations`](#mathematical-operations)
        * [`Why do we use -- ?`](#why-do-we-use)
* [`Conventions`](#conventions)

### Basic Syntax
As said earlier, the syntax of `tragex` is super simple. You could learn it in a matter of seconds, but that makes it not as flexible as other languages.

From this section bellow you will find information on every command you could execute. Keep in mind, that tragx is **case-sensitive**, so when it comes to the premade functions, write them with lower-case characters.

#### Declaring a new variable
To declare a new variable we use the `addx` function. It is executed in this order:
```
addx:[number] [variable name];
```

As you can see, we first provide the name of the variable, and after that we provide the variable name. Each sentence ends with a semi-colon.

Here are some examples:
```
addx:17 seventeen;
addx:-32 other_number;
addx:55 another_cool_number;
```
#### Removing a variable
In tragex you should remove a variable by yourself. If the program ends, all variables would automatically deallocate, but if you want to remove a variable from inside of code, there is a premade function for that - `rmx`:
```
rmx: [variable name];
```
Here are some examples, where we are going to deallocate the variables we made up earlier:
```
rmx: seventeen;
rmx: other_number;
rmx: another_cool_number;
```
As the language is relatively small, there is no way *(and no need)* to inspect that the number is removed.

#### Mathematical operations
In `tragex` there are only two mathematical operations - summation and subtraction. They both have their own keywords.

1. Summation - `sumx`
2. Subtraction - `subx`

They are relatively easy to remember, as the difference is only one letter. Here is how you use them:
```
[operation]:[first_number|variable] -- [second_number|variable]
```

Here are some examples in order to understand it better:
```
sumx: 17 -- 13;
subx: 12 -- 10;

addx: 20 twenty;
addx: 30 thirty;

sumx: twenty -- thirty;

addx: sumx: 17 -- 13 thirty;
```

And yes, you could use variables instead of numbers.

##### Why do we use `--`?
I have no explanation, it just looks cool.


### Conventions
There are a few conventions in tragex, that are made up of the main developer, so the code looks way cleaner.
1. `Snake Case` - In tragex we use [`snake_case`](https://en.wikipedia.org/wiki/Snake_case). It does not mean, that [`PascalCase`](https://www.techopedia.com/definition/pascal-case) or [`camelCase`](https://en.wikipedia.org/wiki/Camel_case).