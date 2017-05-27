## Programming guide

Using the kdtree library in your programs is very easy.

### Creating a tree

Call the **kd_create** function to create a tree. It takes a single argument specifying the dimensionality of the data, and returns a pointer to the tree. You need to pass that pointer as an argument to any functions that manipulate the kd-tree.

For example you may create a 3-dimensional kd-tree with:

```C
void *kd = kd_create(3);
```

### Destroying a tree

Call **kd_free** with a pointer returned from kd_create in order to free the memory occupied by the tree. Note that any data pointers passed by the user with the kd_insert functions will not be freed, unless a "data destructor" function is provided (see below).

### Managing user pointers

When inserting data to the tree, you may pass a pointer to be stored in the node. If you wish, you may provide a custom "destructor" function to be called for each of these pointers when their node is removed from the tree. You can do that by supplying a pointer to that destructor function with a call to kd_data_destructor. The first argument is again a valid pointer to a kd-tree, while the second argument is a function pointer with the signature: void (*)(void*).

### Populating the tree

To insert data to the kd-tree you may use one of the kd_insert functions.

All of the insertion functions take a valid tree pointer as their first argument, and an optional pointer to user data to be stored along with the node as their last argument (it can be null if no user data are needed).

**kd_insert**, and **kd_insertf** expect a pointer to an array of k doubles or floats respectively as a second argument, which contain the position of the inserted point. So for example, for a 3D tree you need to pass an array of 3 values.

The convenience **kd_insert3**, and **kd_insert3f** are meant to be called for 3-dimensional kd-trees (which is considered the most common case), and expect 3 values (doubles or floats) signifying the position of the 3-dimensional point to be stored.

### Performing nearest-neighbor queries

After you have your data in the kd-tree, you can perform queries for discoverying nearest neighbors in a given range around an arbitrary point. The query returns a pointer to the "result set", which can from then on be accessed with the **kd\_res\_\*** functions.

The nearest-neighbor queries are performed with the **kd_nearest_range** functions. Like the **kd_insert** functions described above, they also provide generic array argument versions for k-dimensional trees, and 3-dimensional convenience functions, all in double and float varieties.

For example in order to query for the nearest neighbors around the 2D point (10, 15), inside a radius of 3 units, you could do:

```C
void *result_set;
double pt[] = {10.0, 15.0};
result_set = kd_nearest_range(kd, pt, 3.0);
```
where "kd" is a pointer to a 2-dimensional kd-tree returned by kd_create(2).

A result set aquired with one of the kd_nearest_range functions, must be freed by calling **kd_res_free** and supplying the result set pointer as its only argument.

to be continued...
