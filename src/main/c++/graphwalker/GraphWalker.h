#ifndef GRAPHWALKER_H
#define GRAPHWALKER_H

class GrapwWalker {
  public:
    enum Keywords { REQTAG = 1,
        START_NODE = 2,
        STOP_NODE = 4,
        BLOCKED = 8,
        SWITCH_MODEL = 16
    };
};

#endif
