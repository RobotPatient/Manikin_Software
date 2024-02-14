

# Struct ArgSpecs



[**ClassList**](annotated.md) **>** [**ArgSpecs**](structArgSpecs.md)



_Struct used to pass options for argument parsing between methods._ [More...](#detailed-description)

* `#include <service_protocol.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  int | [**lower\_range**](#variable-lower_range)  <br> |
|  uint8\_t | [**num\_of\_arguments**](#variable-num_of_arguments)  <br> |
|  int | [**upper\_range**](#variable-upper_range)  <br> |












































# Detailed Description




**Note:**

num\_of\_arguments is the number of arguments to check 




**Note:**

upper\_range is the highest integer number the argument can be 




**Note:**

lower\_range is the lowest integer number the argument can be




**Note:**

The argument has to be in between these two numbers! 





    
## Public Attributes Documentation




### variable lower\_range 

```C++
int ArgSpecs::lower_range;
```






### variable num\_of\_arguments 

```C++
uint8_t ArgSpecs::num_of_arguments;
```






### variable upper\_range 

```C++
int ArgSpecs::upper_range;
```




------------------------------
The documentation for this class was generated from the following file `SensorHub_FW/src/service_protocol.hpp`

