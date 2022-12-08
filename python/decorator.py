T = TypeVar("T", bound=object)

class Decorator(Generic[T], object):

    def __init__(self, _decorated: T) -> None:
        self._decorated = _decorated
        self._decorated_attributes = [x for x in dir(_decorated) if not x.startswith("__")]

    def __getattr__(self, attribute: str) -> object:
        if attribute in self._decorated_attributes:
            return getattr(self._decorated, attribute)
        with suppress(AttributeError):
            # In case of multiple decorators.
            return getattr(self._decorated._decorated, attribute)
        return getattr(object, attribute)
    
    def __str__(self) -> str:
        return self._decorated.__str__()
    
    def undecorated(self) -> T:
        return self._decorated
