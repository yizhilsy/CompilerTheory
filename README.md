## LL(1)分析法

### 文法定义：

$$
G[E]:\\
E->I|AIR\\
R->\epsilon|AIR\\
I->FO\\
O->\epsilon|MFO\\
F->b|n|(E)\\
A->+|-
M->*|/
$$

### 预测分析表

|      | b     | n     | (      | )             | +             | -             | *      | /      | #             |
| ---- | ----- | ----- | ------ | ------------- | ------------- | ------------- | ------ | ------ | ------------- |
| E    | E->I  | E->I  | E->I   |               | E->AIR        | E->AIR        |        |        |               |
| R    |       |       |        | $E->\epsilon$ | R->AIR        | R->AIR        |        |        | $R->\epsilon$ |
| I    | I->FO | I->FO | I->FO  |               |               |               |        |        |               |
| O    |       |       |        | $O->\epsilon$ | $O->\epsilon$ | $O->\epsilon$ | O->MFO | O->MFO | $O->\epsilon$ |
| F    | F->b  | F->n  | F->(E) |               |               |               |        |        |               |
| A    |       |       |        |               | A->+          | A->-          |        |        |               |
| M    |       |       |        |               |               |               | M->*   | M->/   |               |

