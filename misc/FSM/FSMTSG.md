## .xml格式状态机描述文件分析

依据文件`State.h State.cpp FSM.h FSM.cpp`

**State**

```c++
class State {
    string Name;
    map<string, string> NextState;		// <input, nextstate_name>
    map<string, string> Output;			// <input, output>
}
```

**FSM**

```c++
class FSM {
    string name;
    string initStateName;
    set<string> AllOutput;
    set<string> AllInput;
    map<string, State> AllState;			// <state_name, class State>
    map<pair<string, string>, pair<string, string>> allTrans;
    map<string, State> transtions_in;
    set<string> inputSet;
    map<string, map<string, State>*> inputStateMap;
    map<string, vector<vector<string>>> stateCoverSet;
}
```



```xml
<fsm>
    <name>main</name>
    <states>3,cm1,cm2,s0</states>
    <init_state>s0</init_state>
    <inputs>4,recorder,poweroff,poweron,off</inputs>
    <outputs>4,recorder_ok,poweroff_ok,poweron_ok,o</outputs>
    <transition>s0,cm2,poweron,poweron_ok,cm1,cm2,off,o</transition>
    <transition_in state="cm2">s2-1,cm1,recorder,recorder_ok,s2-2,s0,poweroff,poweroff_ok</transition_in>
  </fsm>
```



- `<states>3,cm1,cm2,s0</states>`第一个数字是状态的个数，后跟状态名称，读入存储到`FSM::AllState`
- `<init_state>s0</init_state>`顾名思义，初始状态名称

- `<inputs>4,recorder,poweroff,poweron,off</inputs>`个数，后跟输入的字符串。在`readInput`函数读入时，初始化了`AllState`中的每个状态，具体的，将每个输入所产生的输出设置为`string("")`，将每个输入对应的转移指向自身。
- `<outputs>4,recorder_ok,poweroff_ok,poweron_ok,o</outputs>`个数，后跟输出的字符串。记录到`AllOutput`中

- `<transition>s0,cm2,poweron,poweron_ok,cm1,cm2,off,o</transition>`每四个一组，分别对应`source_state, dest_state, input, output`。`readTrans`函数更新`AllState`中的`State`。
- `<transition_in state="cm2">s2-1,cm1,recorder,recorder_ok,s2-2,s0,poweroff,poweroff_ok</transition_in>`子状态转移，例：cm2中的s2-1状态转移到cm1，因recorder输入，输出recorder_ok。

## K等价均分

`firstPartition`相同输出的分一个组