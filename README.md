GraphWalkerEditor
=================

A GraphWalker Editor

Using this editor, you can create models to be used by GraphWalker. 
GraphWalker uses a special syntax, which the editor will enforce.

See http://graphwalker.org/docs

###Dependencies###

Following development libraries need to be installed.

OGDF - https://github.com/ogdf/ogdf

QT4 - http://qt-project.org/

###Building###


```bash
# Clone the repo
git clone https://github.com/KristianKarl/GraphWalkerEditor
cd GraphWalkerEditor
mkdir build
cd build

# Build it
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Run the editor
./gwe

# Run the unit tests
./gwe_unit_test
```
