### How to use class Decorator 

*(NB: Do not confuse with Python's @ decorators).*

First, you need to get a base class, e.g., Box.

```Python
class Box:

    def __init__(self, _material: str) -> None:
        self._material = _material
        self._items: list[object] = []

    @property
    def material(self) -> str:
        return self._material

    def put(self, item: object) -> None:
        self._items.append(item)
        print(f"{item} has been put in the box.")
```

Next, you create a class decorator like this:

```Python
class BoxWithColor(Decorator[Box]):

    def __init__(self, _box: Box, _color: str) -> None:
        super().__init__(_box)  # Calling Decorator's constructor is mandatory to get access to Box' attributes.
        self._color = _color
    
    @property
    def color(self) -> str:
        return self._color
```

, and use it like this:

```Python
box = Box("wood")
box = BoxWithColor(box, "brown")

print(box.material)   # wood
print(box.color)      # brown
box.put("a pen")      # a pen has been put in the box
```

You can create and stack on top of each other any possible number of decorators.

```Python
class BoxWithCapacity(Decorator[Box]):

    def __init__(self, _box: Box, _capacity: int) -> None:
        super().__init__(_box)
        self._capacity = _capacity

    @property
    def capacity(self) -> int: 
        return self._capacity

    @override
    def put(self, item: object) -> None:
        if len(self._items) >= self.capacity:
            print(f"Failed to put {item} in the box, the box is full.")
            return
            
        self._decorated.put(item)
        
# In use.
box = BoxWithCapacity(box, 2)

print(box.material)   # wood
print(box.color)      # brown
print(box.capacity)   # 2

box.put("a ball")     # a ball has been put in the box.
box.put("a jacket")   # Failed to put a jacket in the box, the box is full.

```

### How to get some of the editor and type checker support back

A notable drawback of using class decorators is the loss of support from the code editor / environment, namely: 

  * Suggestions on automatic code completion.
  * Static type checking.
  * Renaming all attribute occurences across the entire project.

There are workarounds, although they do not look very clean and have some limitations.

*If you simply want to annotate a decorated object*, a possible solution would be **to create a new annotation class (1)** which inherits the decorators and the base object.

```Python
@final  # Make it clear that this class is not meant to be used in other context.
class BoxWithColorAndCapacity(BoxWithColor, BoxWithCapacity, Box): pass

box: BoxWithColorAndCapacity = BoxWithColor(BoxWithCapacity(Box("wood"), 20), "red")
```

*If you want to annotate within a decorator's body that it has all attributes of the decorated object*, you can **annotate self (2)**.

```Python
class BoxWithCapacity(Decorator[Box]):

    @override
    def put(self: Box | BoxWithCapacity, item: object) -> None: 
        if len(self._items) >= self.capacity:   # Here, _items will be properly recognized as a Box' attribute. 
            print(f"Failed to put {item} in the box, the box is full.")
            return
            
        self._decorated.put(item)
```
The third method probably deserves the biggest attention. 

There might be a common case when you want to *use your decorator with interfaces*. 

Let's say, your Box is now an interface and its previous body is now one of its ConcreteBox implementation.

```Python
class Box(Protocol):
    @abstractmethod
    def put(self, item: object) -> None: ...

class ConcreteBox(Box):

    def __init__(self, _material: str) -> None:
        # View the previous Box' body.
```

If you  decide to annotate a decorator as implementing the Box class, you'll get some nasty results:

```Python
class BoxWithColor(Decorator[Box], Box):    # Don't!

    def __init__(self, _box: Box, _color: str) -> None:
        ...

box = BoxWithColor(ConcreteBox("wood"), "red")  # TypeError: Can't instantiate abstract class BoxWithColor with abstract methods material, put
```

This is because "to implement a protocol in Python" in fact means to inherit the class, so your decorator will try to use abstract attributes of its "parent" rather than getting them from the decorated object.

What you might do, is **to provide fake inheritance for the type checker only which will not be used during run-time (3)**.

```Python
if TYPE_CHECKING:
    class BoxWithColorAnnotation(Box): pass
else:
    class BoxWithColorAnnotation: pass

class BoxWithColor(Decorator[Box], BoxWithColorAnnotation): ...

box = BoxWithColor(ConcreteBox("wood"), "red")  # Works fine during run-time and is recognized by the type checker.
```

Combining the methods **(1)** and **(3)** is probably the most efficient way to get maximum from your editor and type checker without preventing your code from working properly.
