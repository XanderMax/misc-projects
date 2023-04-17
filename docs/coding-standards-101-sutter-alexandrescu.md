This page reproduces content from the _Summary of Summaries_ chapter of **C++ Coding Standards: 101 Rules, Guidelines, and Best Practices**, by Herb Sutter and Andrei Alexandrescu, published in 2005 by Pearson Education, Inc.

<div style="clear: both;"></div>

## Organizational and Policy Issues

### 0. Don’t sweat the small stuff. (Or: Know what not to standardize.)

Say only what needs saying: Don’t enforce personal tastes or obsolete practices.

### 1. Compile cleanly at high warning levels.

Take warnings to heart: Use your compiler’s highest warning level. Require clean (warning-free) builds. Understand all warnings. Eliminate warnings by changing your code, not by reducing the warning level.

### 2. Use an automated build system.

Push the (singular) button: Use a fully automatic (“one-action”) build system that builds the whole project without user intervention.

### 3. Use a version control system.

The palest of ink is better than the best memory (Chinese proverb): Use a version control system (VCS). Never keep files checked out for long periods. Check in frequently after your updated unit tests pass. Ensure that checked-in code does not break the build.

### 4. Invest in code reviews.

Re-view code: More eyes will help make more quality. Show your code, and read others’. You’ll all learn and benefit.

## Design Style

{:start="5"}
### 5. Give one entity one cohesive responsibility.

Focus on one thing at a time: Prefer to give each entity (variable, class, function, namespace, module, library) one well-defined responsibility. As an entity grows, its scope of responsibility naturally increases, but its responsibility should not diverge.

### 6. Correctness, simplicity, and clarity come first.

KISS (Keep It Simple Software): Correct is better than fast. Simple is better than complex. Clear is better than cute. Safe is better than insecure (see Items 83 and 99).

### 7. Know when and how to code for scalability.

Beware of explosive data growth: Without optimizing prematurely, keep an eye on asymptotic complexity. Algorithms that work on user data should take a predictable, and preferably no worse than linear, time with the amount of data processed. When optimization is provably necessary and important, and especially if it’s because data volumes are growing, focus on improving big-Oh complexity rather than on micro-optimizations like saving that one extra addition.

### 8. Don’t optimize prematurely.

Spur not a willing horse (Latin proverb): Premature optimization is as addictive as it is unproductive. The first rule of optimization is: Don’t do it. The second rule of optimization (for experts only) is: Don’t do it yet. Measure twice, optimize once.

### 9. Don’t pessimize prematurely.

Easy on yourself, easy on the code: All other things being equal, notably code complexity and readability, certain efficient design patterns and coding idioms should just flow naturally from your fingertips and are no harder to write than the pessimized alternatives. This is not premature optimization; it is avoiding gratuitous pessimization.

### 10. Minimize global and shared data.

Sharing causes contention: Avoid shared data, especially global data. Shared data increases coupling, which reduces maintainability and often performance.

### 11. Hide information.

Don’t tell: Don’t expose internal information from an entity that provides an abstraction.

### 12. Know when and how to code for concurrency.

Th<sub>sa</sub>rea<sub>fe</sub>d<sub>ly</sub>: If your application uses multiple threads or processes, know how to minimize sharing objects where possible (see Item 10) and share the right ones safely.

### 13. Ensure resources are owned by objects. Use explicit RAII and smart pointers.

Don’t saw by hand when you have power tools: C++’s “resource acquisition is initialization” (RAII) idiom is the power tool for correct resource handling. RAII allows the compiler to provide strong and automated guarantees that in other languages require fragile hand-coded idioms. When allocating a raw resource, immediately pass it to an owning object. Never allocate more than one resource in a single statement.

## Coding Style

### 14. Prefer compile- and link-time errors to run-time errors.

Don’t put off ’til run time what you can do at build time: Prefer to write code that uses the compiler to check for invariants during compilation, instead of checking them at run time. Run-time checks are control- and data-dependent, which means you’ll seldom know whether they are exhaustive. In contrast, compile-time checking is not control- or data-dependent and typically offers higher degrees of confidence.

### 15. Use `const` proactively.

`const` is your friend: Immutable values are easier to understand, track, and reason about, so prefer constants over variables wherever it is sensible and make `const` your default choice when you define a value: It’s safe, it’s checked at compile time (see Item 14), and it’s integrated with C++’s type system. Don’t cast away `const` except to call a const-incorrect function (see Item 94).

### 16. Avoid macros.

**TO PUT IT BLUNTLY**: Macros are the bluntest instrument of C and C++’s abstraction facilities, ravenous wolves in functions’ clothing, hard to tame, marching to their own beat all over your scopes. Avoid them.

### 17. Avoid magic numbers.

Programming isn’t magic, so don’t incant it: Avoid spelling literal constants like `42` or `3.14159` in code. They are not self-explanatory and complicate maintenance by adding a hard-to-detect form of duplication. Use symbolic names and expressions instead, such as `width * aspectRatio`.

### 18. Declare variables as locally as possible.

Avoid scope bloat, as with requirements so too with variables: Variables introduce state, and you should have to deal with as little state as possible, with lifetimes as short as possible. This is a specific case of Item 10 that deserves its own treatment.

### 19. Always initialize variables.

Start with a clean slate: Uninitialized variables are a common source of bugs in C and C++ programs. Avoid such bugs by being disciplined about cleaning memory before you use it; initialize variables upon definition.

### 20. Avoid long functions. Avoid deep nesting.

Short is better than long, flat is better than deep: Excessively long functions and nested code blocks are often caused by failing to give one function one cohesive responsibility (see Item 5), and both are usually solved by better refactoring.

### 21. Avoid initialization dependencies across compilation units.

Keep (initialization) order: Namespace-level objects in different compilation units should never depend on each other for initialization, because their initialization order is undefined. Doing otherwise causes headaches ranging from mysterious crashes when you make small changes in your project to severe nonportability even to new releases of the same compiler.

### 22. Minimize definitional dependencies. Avoid cyclic dependencies.

Don’t be over-dependent: Don’t `#include` a definition when a forward declaration will do.

Don’t be co-dependent: Cyclic dependencies occur when two modules depend directly or indirectly on one another. A module is a cohesive unit of release (see page 103); modules that are interdependent are not really individual modules, but super-glued together into what’s really a larger module, a larger unit of release. Thus, cyclic dependencies work against modularity and are a bane of large projects. Avoid them.

### 23. Make header files self-sufficient.

Behave responsibly: Ensure that each header you write is compilable standalone, by having it include any headers its contents depend upon.

### 24. Always write internal `#include` guards. Never write external `#include` guards.

Wear head(er) protection: Prevent unintended multiple inclusions by using `#include` guards with unique names for all of your header files.

## Functions and Operators

### 25. Take parameters appropriately by value, (smart) pointer, or reference.

Parameterize well: Distinguish among input, output, and input/output parameters, and between value and reference parameters. Take them appropriately.

### 26. Preserve natural semantics for overloaded operators.

Programmers hate surprises: Overload operators only for good reason, and preserve natural semantics; if that’s difficult, you might be misusing operator overloading.

### 27. Prefer the canonical forms of arithmetic and assignment operators.

If you `a+b`, also `a+=b`: When defining binary arithmetic operators, provide their assignment versions as well, and write to minimize duplication and maximize efficiency.

### 28. Prefer the canonical form of `++` and `--` . Prefer calling the prefix forms.

If you `++c`, also `c++`: The increment and decrement operators are tricky because each has pre- and postfix forms, with slightly different semantics. Define `operator++` and `operator--` such that they mimic the behavior of their built-in counterparts. Prefer to call the prefix versions if you don’t need the original value.

### 29. Consider overloading to avoid implicit type conversions.

Do not multiply objects beyond necessity (Occam’s Razor): Implicit type conversions provide syntactic convenience (but see Item 40). But when the work of creating temporary objects is unnecessary and optimization is appropriate (see Item 8), you can provide overloaded functions with signatures that match common argument types exactly and won’t cause conversions.

### 30. Avoid overloading `&&`, `||`, or `,`(comma).

Wisdom means knowing when to refrain: The built-in `&&,` `||`, and `,` (comma) enjoy special treatment from the compiler. If you overload them, they become ordinary functions with very different semantics (you will violate Items 26 and 31), and this is a sure way to introduce subtle bugs and fragilities. Don’t overload these operators naïvely.

### 31. Don’t write code that depends on the order of evaluation of function arguments.

Keep (evaluation) order: The order in which arguments of a function are evaluated is unspecified, so don’t rely on a specific ordering.

## Class Design and Inheritance

### 32. Be clear what kind of class you’re writing.

Know thyself: There are different kinds of classes. Know which kind you are writing.

### 33. Prefer minimal classes to monolithic classes.

Divide and conquer: Small classes are easier to write, get right, test, and use. They are also more likely to be usable in a variety of situations. Prefer such small classes that embody simple concepts instead of kitchen-sink classes that try to implement many and/or complex concepts (see Items 5 and 6).

### 34. Prefer composition to inheritance.

Avoid inheritance taxes: Inheritance is the second-tightest coupling relationship in C++, second only to friendship. Tight coupling is undesirable and should be avoided where possible. Therefore, prefer composition to inheritance unless you know that the latter truly benefits your design.

### 35. Avoid inheriting from classes that were not designed to be base classes.

Some people don’t want to have kids: Classes meant to be used standalone obey a different blueprint than base classes (see Item 32). Using a standalone class as a base is a serious design error and should be avoided. To add behavior, prefer to add nonmember functions instead of member functions (see Item 44). To add state, prefer composition instead of inheritance (see Item 34). Avoid inheriting from concrete base classes.

### 36. Prefer providing abstract interfaces.

Love abstract art: Abstract interfaces help you focus on getting an abstraction right without muddling it with implementation or state management details. Prefer to design hierarchies that implement abstract interfaces that model abstract concepts.

### 37. Public inheritance is substitutability. Inherit, not to reuse,but to be reused.

Know what: Public inheritance allows a pointer or reference to the base class to actually refer to an object of some derived class, without destroying code correctness and without needing to change existing code. Know why: Don’t inherit publicly to reuse code (that exists in the base class); inherit publicly in order to be reused (by existing code that already uses base objects polymorphically).

### 38. Practice safe overriding.

Override responsibly: When overriding a virtual function, preserve substitutability; in particular, observe the function’s pre- and post-conditions in the base class. Don’t change default arguments of virtual functions. Prefer explicitly redeclaring overrides as virtual. Beware of hiding overloads in the base class.

### 39. Consider making virtual functions nonpublic, and public functions nonvirtual.

In base classes with a high cost of change (particularly ones in libraries and frameworks): Prefer to make public functions nonvirtual. Prefer to make virtual functions private, or protected if derived classes need to be able to call the base versions. (Note that this advice does not apply to destructors; see Item 50.)

### 40. Avoid providing implicit conversions.

Not all change is progress: Implicit conversions can often do more damage than good. Think twice before providing implicit conversions to and from the types you define, and prefer to rely on explicit conversions (`explicit` constructors and named conversion functions).

### 41. Make data members private, except in behaviorless aggregates (C-style `struct`s).

They’re none of your caller’s business: Keep data members private. Only in the case of simple C-style `struct` types that aggregate a bunch of values but don’t pretend to encapsulate or provide behavior, make all data members public. Avoid mixes of public and nonpublic data, which almost always signal a muddled design.

### 42. Don’t give away your internals.

Don’t volunteer too much: Avoid returning handles to internal data managed by your class, so clients won’t uncontrollably modify state that your object thinks it owns.

### 43. Pimpl judiciously.

Overcome the language’s separation anxiety: C++ makes private members inaccessible, but not invisible. Where the benefits warrant it, consider making private members truly invisible using the Pimpl idiom to implement compiler firewalls and increase information hiding. (See Items 11 and 41.)

### 44. Prefer writing nonmember nonfriend functions.

Avoid membership fees: Where possible, prefer making functions nonmember nonfriends.

### 45. Always provide `new` and `delete` together.

They’re a package deal: Every class-specific overload `void* operator new(parms)` must be accompanied by a corresponding overload `void operator delete(void*, parms)`, where `parms` is a list of extra parameter types (of which the first is always `std::size_t`). The same goes for the array forms `new[]` and `delete[]`.

### 46. If you provide any class-specific `new`, provide all of the standard forms (plain, in-place, and `nothrow`).

Don’t hide good `new`s: If a class defines any overload of `operator new`, it should provide overloads of all three of plain, in-place, and non-throwing `operator new`. If you don’t, they’ll be hidden and unavailable to users of your class.

## Construction, Destruction, and Copying

### 47. Define and initialize member variables in the same order.

Agree with your compiler: Member variables are always initialized in the order they are declared in the class definition; the order in which you write them in the constructor initialization list is ignored. Make sure the constructor code doesn’t confusingly specify a different order.

### 48. Prefer initialization to assignment in constructors.

Set once, use everywhere: In constructors, using initialization instead of assignment to set member variables prevents needless run-time work and takes the same amount of typing.

### 49. Avoid calling virtual functions in constructors and destructors.

Virtual functions only “virtually” always behave virtually: Inside constructors and destructors, they don’t. Worse, any direct or indirect call to an unimplemented pure virtual function from a constructor or destructor results in undefined behavior. If your design wants virtual dispatch into a derived class from a base class constructor or destructor, you need other techniques such as post-constructors.

### 50. Make base class destructors public and virtual, or protected and nonvirtual.

To delete, or not to delete; that is the question: If deletion through a pointer to a base Base should be allowed, then Base’s destructor must be public and virtual. Otherwise, it should be protected and nonvirtual.

### 51. Destructors, deallocation, and `swap` never fail.

Everything they attempt shall succeed: Never allow an error to be reported from a destructor, a resource deallocation function (e.g., `operator delete`), or a swap function. Specifically, types whose destructors may throw an exception are flatly forbidden from use with the C++ standard library.

### 52. Copy and destroy consistently.

What you create, also clean up: If you define any of the copy constructor, copy assignment operator, or destructor, you might need to define one or both of the others.

### 53. Explicitly enable or disable copying.

Copy consciously: Knowingly choose among using the compiler-generated copy constructor and assignment operator, writing your own versions, or explicitly disabling both if copying should not be allowed.

### 54. Avoid slicing. Consider `Clone` in stead of copying in base classes.

Sliced bread is good; sliced objects aren’t: Object slicing is automatic, invisible, and likely to bring wonderful polymorphic designs to a screeching halt. In base classes, consider disabling the copy constructor and copy assignment operator, and instead supplying a virtual `Clone` member function if clients need to make polymorphic (complete, deep) copies.

### 55. Prefer the canonical form of assignment.

Your assignment: When implementing `operator=`, prefer the canonical form—nonvirtual and with a specific signature.

### 56. Whenever it makes sense, provide a no-fail swap (and provide it correctly).

`swap` is both a lightweight and a workhorse: Consider providing a `swap` function to efficiently and infallibly swap the internals of this object with another’s. Such a function can be handy for implementing a number of idioms, from smoothly moving objects around to implementing assignment easily to providing a guaranteed commit function that enables strongly error-safe calling code. (See also Item 51.)

## Namespaces and Modules

### 57. Keep a type and its nonmember function interface in the same namespace.

Nonmembers are functions too: Nonmember functions that are designed to be part of the interface of a class X (notably operators and helper functions) must be defined in the same namespace as the X in order to be called correctly.

### 58. Keep types and functions in separate namespaces unless they’re specifically intended to work together.

Help prevent name lookup accidents: Isolate types from unintentional argument-dependent lookup (ADL, also known as Koenig lookup), and encourage intentional ADL, by putting them in their own namespaces (along with their directly related nonmember functions; see Item 57). Avoid putting a type into the same namespace as a templated function or operator.

### 59. Don’t write namespace `using`s in a header file or before an `#include`.

Namespace `using`s are for your convenience, not for you to inflict on others: Never write a using declaration or a `using` directive before an `#include` directive.

Corollary: In header files, don’t write namespace-level `using` directives or `using` declarations; instead, explicitly namespace-qualify all names. (The second rule follows from the first, because headers can never know what other header `#includes` might appear after them.)

### 60. Avoid allocating and deallocating memory in different modules.

Put things back where you found them: Allocating memory in one module and deallocating it in a different module makes your program fragile by creating a subtle long-distance dependency between those modules. They must be compiled with the same compiler version and same flags (notably debug vs. `NDEBUG`) and the same standard library implementation, and in practice the module allocating the memory had better still be loaded when the deallocation happens.

### 61. Don’t define entities with linkage in a header file.

Repetition causes bloat: Entities with linkage, including namespace-level variables or functions, have memory allocated for them. Defining such entities in header files results in either link-time errors or memory waste. Put all entities with linkage in implementation files.

### 62. Don’t allow exceptions to propagate across module boundaries.

Don’t throw stones into your neighbor’s garden: There is no ubiquitous binary standard for C++ exception handling. Don’t allow exceptions to propagate between two pieces of code unless you control the compiler and compiler options used to build both sides; otherwise, the modules might not support compatible implementations for exception propagation. Typically, this boils down to: Don’t let exceptions propagate across module/subsystem boundaries.

### 63. Use sufficiently portable types in a module’s interface.

Take extra care when living on the edge (of a module): Don’t allow a type to appear in a module’s external interface unless you can ensure that all clients understand the type correctly. Use the highest level of abstraction that clients can understand.

## Templates and Genericity

### 64. Blend static and dynamic polymorphism judiciously.

So much more than a mere sum of parts: Static and dynamic polymorphism are complementary. Understand their tradeoffs, use each for what it’s best at, and mix them to get the best of both worlds.

### 65. Customize intentionally and explicitly.

Intentional is better than accidental, and explicit is better than implicit: When writing a template, provide points of customization knowingly and correctly, and document them clearly. When using a template, know how the template intends for you to customize it for use with your type, and customize it appropriately.

### 66. Don’t specialize function templates.

Specialization is good only when it can be done correctly: When extending someone else’s function template (including `std::swap`), avoid trying to write a specialization; instead, write an overload of the function template, and put it in the namespace of the type(s) the overload is designed to be used for. (See Item 57.) When you write your own function template, avoid encouraging direct specialization of the function template itself.

### 67. Don't write unintentionally nongeneric code.

Commit to abstractions, not to details: Use the most generic and abstract means to implement a piece of functionality.

## Error Handling and Exceptions

### 68. Assert liberally to document internal assumptions and invariants.

Be assertive! Use `assert` or an equivalent liberally to document assumptions internal to a module (i.e., where the caller and callee are maintained by the same person or team) that must always be true and otherwise represent programming errors (e.g., violations of a function’s postconditions detected by the caller of the function). (See also Item 70.) Ensure that assertions don’t perform side effects.

### 69. Establish a rational error handling policy, and follow it strictly.

Consciously specify, and conscientiously apply, what so many projects leave to ad-hoc (mis)judgment: Develop a practical, consistent, and rational error handling policy early in design, and then stick to it. Ensure that it includes:

* Identification: What conditions are errors.
* Severity: How important or urgent each error is.
* Detection: Which code is responsible for detecting the error.
* Propagation: What mechanisms are used to report and propagate error notifications in each module.
* Handling: What code is responsible for doing something about the error.
* Reporting: How the error will be logged or users notified. Change error handling mechanisms only on module boundaries.

### 70. Distinguish between errors and non-errors.

A breach of contract is an error: A function is a unit of work. Thus, failures should be viewed as errors or otherwise based on their impact on functions. Within a function `f`, a failure is an error if and only if it violates one of `f`’s preconditions or prevents `f` from meeting any of its callees’ preconditions, achieving any of `f`’s own postconditions, or reestablishing any invariant that `f` shares responsibility for maintaining.

In particular, here we exclude internal programming errors (i.e., where the caller and callee are the responsibility of the same person or team, such as inside a module), which are a separate category normally dealt with using assertions (see Item 68).

### 71. Design and write error-safe code.

Promise, but don’t punish: In each function, give the strongest safety guarantee that won’t penalize callers who don’t need it. Always give at least the basic guarantee.

Ensure that errors always leave your program in a valid state. This is the basic guarantee. Beware of invariant-destroying errors (including but not limited to leaks), which are just plain bugs.

Prefer to additionally guarantee that the final state is either the original state (if there was an error the operation was rolled back) or the intended target state (if there was no error the operation was committed). This is the strong guarantee.

Prefer to additionally guarantee that the operation can never fail at all. Although this is not possible for most functions, it is required for functions like destructors and deallocation functions. This is the no-fail guarantee.

### 72. Prefer to use exceptions to report errors.

When harmed, take exception: Prefer using exceptions over error codes to report errors. Use status codes (e.g., return codes, `errno`) for errors when exceptions cannot be used (see Item 62), and for conditions that are not errors. Use other methods, such as graceful or ungraceful termination, when recovery is impossible or not required.

### 73. Throw by value, catch by reference.

Learn to `catch` properly: Throw exceptions by value (not pointer) and catch them by reference (usually to `const`). This is the combination that meshes best with exception semantics. When rethrowing the same exception, prefer just `throw`; to `throw e`;.

### 74. Report, handle, and translate errors appropriately.

Know when to say when: Report errors at the point they are detected and identified as errors. Handle or translate each error at the nearest level that can do it correctly.

### 75. Avoid exception specifications.

Take exception to these specifications: Don’t write exception specifications on your functions unless you’re forced to (because other code you can’t change has already introduced them; see Exceptions).

## STL: Containers

### 76. Use vector by default. Otherwise, choose an appropriate container.

Using the “right container” is great: If you have a good reason to use a specific container type, use that container type knowing that you did the right thing.

So is using `vector`: Otherwise, write `vector` and keep going without breaking stride, also knowing you did the right thing.

### 77. Use `vector` and `string` instead of arrays.

Why juggle Ming vases? Avoid implementing array abstractions with C-style arrays, pointer arithmetic, and memory management primitives. Using `vector` or `string` not only makes your life easier, but also helps you write safer and more scalable software.

### 78. Use `vector` (and `string::c_str`) to exchange data with non-C++ APIs.

`vector` isn’t lost in translation: `vector` and `string::c_str` are your gateway to communicate with non-C++ APIs. But don’t assume iterators are pointers; to get the address of the element referred to by a `vector<T>::iterator iter`, use `&*iter`.

### 79. Store only values and smart pointers in containers.

Store objects of value in containers: Containers assume they contain value-like types, including value types (held directly), smart pointers, and iterators.

### 80. Prefer push back to other ways of expanding a sequence.

push back all you can: If you don’t need to care about the insert position, prefer using push back to add an element to a sequence. Other means can be both vastly slower and less clear.

### 81. Prefer range operations to single-element operations.

Don’t use oars when the wind is fair (based on a Latin proverb): When adding elements to sequence containers, prefer to use range operations (e.g., the form of insert that takes a pair of iterators) instead of a series of calls to the single-element form of the operation. Calling the range operation is generally easier to write, easier to read, and more efficient than an explicit loop. (See also Item 84.)

### 82. Use the accepted idioms to really shrink capacity and really erase elements.

Use a diet that works: To really shed excess capacity from a container, use the “swap trick.” To really erase elements from a container, use the erase-remove idiom.

## STL: Algorithms

### 83. Use a checked STL implementation.

Safety first (see Item 6): Use a checked STL implementation, even if it’s only available for one of your compiler platforms, and even if it’s only used during pre-release testing.

### 84. Prefer algorithm calls to handwritten loops.

Use function objects judiciously: For very simple loops, handwritten loops can be the simplest and most efficient solution. But writing algorithm calls instead of handwritten loops can be more expressive and maintainable, less error-prone, and as efficient.

When calling algorithms, consider writing your own custom function object that encapsulates the logic you need. Avoid cobbling together parameter-binders and simple function objects (e.g., `bind2nd`, `plus`), which usually degrade clarity. Consider trying the [Boost] Lambda library, which automates the task of writing function objects.

### 85. Use the right STL search algorithm.

Search “just enough”—the right search may be STL (slower than light), but it’ll still be pretty fast: This Item applies to searching for a particular value in a range, or for the location where it would be if it were in the range. To search an unsorted range, use `find`/`find if` or `count`/`count if`. To search a sorted range, use lower bound, upper bound, equal range, or (rarely) binary search. (Despite its common name, binary search is usually not the right choice.)

### 86. Use the right STL sort algorithm.

Sort “just enough:” Understand what each of the sorting algorithms does, and use the cheapest algorithm that does what you need.

### 87. Make predicates pure functions.

Predicate purity: A predicate is a function object that returns a yes/no answer, typically as a `bool` value. A function is pure in the mathematical sense if its result depends only on its arguments (note that this use of “pure” has nothing to do with pure virtual functions).

Don’t allow predicates to hold or access state that affects the result of their `operator()`, including both member and global state. Prefer to make `operator()` a `const` member function for predicates (see Item 15).

### 88. Prefer function objects over functions as algorithm and comparer arguments.

Objects plug in better than functions: Prefer passing function objects, not functions, to algorithms. Comparers for associative containers must be function objects. Function objects are adaptable and, counterintuitively, they typically produce faster code than functions.

### 89. Write function objects correctly.

Be cheap, be adaptable: Design function objects to be values that are cheap to copy. Where possible, make them adaptable by inheriting from `unary` - or `binary function`.

## Type Safety

### 90. Avoid type switching; prefer polymorphism.

Switch off: Avoid switching on the type of an object to customize behavior. Use templates and virtual functions to let types (not their calling code) decide their behavior.

### 91. Rely on types, not on representations.

Don’t try to X-ray objects (see Item 96): Don’t make assumptions about how objects are exactly represented in memory. Instead, let types decide how their objects are written to and read from memory.

### 92. Avoid using `reinterpret_cast`.

Lies have short legs (German and Romanian proverb): Don’t try to use `reinterpret_cast` to force the compiler to reinterpret the bits of an object of one type as being the bits of an object of a different type. That’s the opposite of maintaining type safety, and `reinterpret_cast` isn’t even guaranteed to do that or anything else in particular.

### 93. Avoid using `static_cast` on pointers.

Pointers to dynamic objects don’t static cast: Safe alternatives range from using dynamic cast to refactoring to redesigning.

### 94. Avoid casting away `const`.

Some fibs are punishable: Casting away const sometimes results in undefined behavior, and it is a staple of poor programming style even when legal.

### 95. Don’t use C-style casts.

Age doesn’t always imply wisdom: C-style casts have different (and often dangerous) semantics depending on context, all disguised behind a single syntax. Replacing C-style casts with C++-style casts helps guard against unexpected errors.

### 96. Don’t `memcpy` or `memcmp` non-PODs.

Don’t try to X-ray objects (see Item 91): Don’t use memcpy and memcmp to copy or compare anything more structured than raw memory.

### 97. Don’t use unions to reinterpret representation.

A deceit is still a lie: Unions can be abused into obtaining a “cast without a cast” by writing one member and reading another. This is more insidious and even less predictable than `reinterpret_cast` (see Item 92).

### 98. Don’t use varargs (ellipsis).

Ellipses cause collapses: The ellipsis is a dangerous carryover from C. Avoid varargs, and use higher-level C++ constructs and libraries instead.

### 99. Don’t use invalid objects. Don’t use unsafe functions.

Don’t use expired medicines: Both invalid objects and historical but unsafe functions wreak havoc on your program’s health.

### 100. Don’t treat arrays polymorphically.

Arrays are ill-adjusted: Treating arrays polymorphically is a gross type error that your compiler will probably remain silent about. Don’t fall into the trap.