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
    def put(self: Box | BoxWithCapacity, item: object) -> None:   # Typehint self to get the editor support.
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

### How to get some of the editor's support back

A notable drawback of using class decorators is the loss of the editor's support, namely: 

  * Suggestions on automatic code completion.
  * Type checking.
  * Renaming all attribute occurences across the entire project.

As a possible workaround, protocols can be used.

Consider making your base class and all its decorators to implement some protocols:

```Python
class HasMaterial(Protocol):
    @property
    def material(self) -> str: ...
class SupportsPut(Protocol):
    def put(self, item: object) -> None: ...
    
class Box(HasMaterial, SupportsPut):
    ...   # Same implementation as in the first part.


class HasColor(Protocol):
    @property
    def color(self) -> str: ...
    
class BoxWithColor(Decorator[Box], HasColor):
    ...
    
class HasCapacity(Protocol):
    @property
    def capacity(self) -> int: ...
    
class BoxWithCapacity(Decorator[Box], HasCapacity):
    ...
```

Now, when you add a certain combination of decorators on a Box object, you can typehint it using their protocols.

```Python

class BoxWithColorAndCapacity(HasCapacity, HasColor, HasMaterial, SupportsPut, Protocol):   # Note that it's a protocol, not an implementation.
    pass
    
box: BoxWithColorAndCapacity = BoxWithCapacity(BoxWithColor(Box("wood"), "brown"), 2)
```

This gives you back suggestions on code completition, type checking, and, partially, renaming attributes across the projects (if you've overriden an existing base class method within your decorators, like we've done with the method `put()` for `BoxWithCapacity` in the example, then you'll have to manually rename such methods within these decorators).
