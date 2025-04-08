# lock-free-bag
A simple lock-free bag implementation.

This lock-free bag currently uses an internal toDelete raw atomic pointer, which functions as an internal bag. This stores all nodes popped from the bag to resolve the ABA problem. It is a simple but memory inefficient solution.
