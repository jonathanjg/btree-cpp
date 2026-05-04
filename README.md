# btree-cpp

A small C++ project to explore **B-trees**, their insertion, search, and traversal operations.  
This project is designed for learning about **database indexing** and **balanced tree structures**.

---

## Features

- B-tree of configurable minimum degree `t`
- Insert, search, and traversal
- (Optional) Can be extended to deletion or disk simulation

---

## What is a B-tree?

A **B-tree** is a **self-balancing search tree** optimized for **storage systems and databases**.  
It maintains sorted data and allows **logarithmic time search, insertion, and deletion**.  

**Key properties of a B-tree of minimum degree `t`:**

1. Every node contains at most `2t - 1` keys.  
2. Every node (except root) contains at least `t - 1` keys.  
3. All leaves appear at the same level.  
4. A non-leaf node with `n` keys has `n+1` children.  
5. Keys within a node are sorted in ascending order.  

---

## Data Structure

Each **node** contains:

- `keys` – an array of values in sorted order  
- `children` – pointers to child nodes (empty if leaf)  
- `isLeaf` – boolean flag to know if it’s a leaf node  
- `t` – minimum degree of the tree  

**Example of a B-tree (t=2):**

```

```
    [10, 20]
   /    |    \
```

[5, 7] [12,15] [25,30]

````

- Root has 2 keys (`10`, `20`) and 3 children.
- Each leaf has 2 keys (between `t-1` and `2t-1`).

---

## Logic of Operations

### 1. **Search**

- Start at root, find the first key greater than or equal to target.  
- If key matches → found.  
- If node is leaf → not found.  
- Else → recurse into the child at that position.  

**Time Complexity:** O(log n)

---

### 2. **Insertion**

**High-level steps:**

1. **Empty tree:** create a root with the key.  
2. **Root full?**  
   - Yes → split root, create new root with median key, recurse to correct child.  
   - No → insert into root or recursively into correct child.  
3. **InsertNonFull(node, key)**:  
   - If leaf → insert key in sorted order inside node.  
   - If internal → find correct child to insert;  
     - If child full → split child first, then insert.  

**SplitChild(node, child):**

- Median key moves up to parent.  
- Left keys stay in original child.  
- Right keys move to a new node.  

This keeps **all nodes partially filled** and the tree **balanced**, ensuring logarithmic height.

---

### 3. **Traversal**

- In-order traversal prints keys in sorted order.
- For each key, traverse left child first (if exists), then the key, then right child.

---

## Example Usage (`main.cpp`)

```cpp
#include "BTree.hpp"

int main() {
    BTree<int> tree(3); // min degree t=3

    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);

    std::cout << "Traversal: ";
    tree.traverse(); // 5 6 10 12 20

    int key = 6;
    if (tree.search(key))
        std::cout << key << " found\n";
    else
        std::cout << key << " not found\n";

    return 0;
}
````

**Output:**

```
Traversal: 5 6 10 12 20
6 found
```

Perfect! Here’s an **updated section for your README** that explains how to create the Graphviz SVG from your B-tree and open it on RHEL 10 using `xdg-open`. You can just paste this into your README.md under a **Visualizing the Tree** section.

---

## Visualizing the B-tree with Graphviz

This project can export the B-tree structure to a **Graphviz `.dot` file** and then convert it into an image (SVG or PNG) for easy visualization.

### Steps:

1. **Run the program to generate the `.dot` file**

```bash
./btree_cpp
````

* This will create a file called `btree.dot` in the working directory.

2. **Convert the `.dot` file to an SVG image**

```bash
dot -Tsvg btree.dot -o btree.svg
```

* `-Tsvg` tells Graphviz to generate an SVG file.
* You can also use `-Tpng` to create a PNG image:

```bash
dot -Tpng btree.dot -o btree.png
```

3. **Open the SVG file on RHEL 10**

```bash
xdg-open btree.svg
```

* This will open the file with the default image viewer for SVG files.
* Replace `btree.svg` with `btree.png` if you generated a PNG instead.

---

### Notes

* For large B-trees (thousands of nodes), **SVG is recommended** because it scales better.
* You can limit the depth of the tree in your `exportToDot` method to avoid overly large diagrams.
* This provides a clear, visual way to verify **node splitting** and tree structure.


---

## Next Steps / Extensions

* Implement **deletion** (requires merging/borrowing keys)
* Implement **range queries** (useful for database indexes)
* Simulate **disk storage** by storing nodes in files
