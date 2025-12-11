# MiniCompiler-Kit 实验报告（Markdown 完整版）

## 目录
- 一、实验项目名称
- 二、实验原理
- 三、实验目的
- 四、实验内容
- 五、实验器材
- 六、实验步骤
- 七、实验数据及结果分析
- 八、总结及心得体会
- 九、改进建议
- 附录 A：文法说明
- 附录 B：记号类别与编码
- 附录 C：输出文件格式规范
- 附录 D：控制台日志规范
- 附录 E：代码关键位置索引

---

## 一、实验项目名称
MiniCompiler-Kit：简易编译器原型（词法分析、语法分析、符号表生成）

---

## 二、实验原理

### 编译流水线
- 词法分析：从源代码逐字符扫描，识别并输出一个由记号（Token）组成的序列。每个记号包含词素（lexeme）、类别码（type）及所在行号。
- 语法分析：基于给定文法，用递归下降方法判定记号序列是否构成合法的程序结构；在遇到错误时记录统一格式的错误信息，并尽量进行容错与恢复。
- 符号表生成：记录程序中的变量和过程/函数的属性，如名称、所属过程、类别（变量/形参）、类型（字符串）、层次（作用域深度）与地址（相对序号），为后续语义分析与目标代码生成奠定基础。

### 词法分析原理
- 关键字识别：通过关键字表匹配 `begin/end/integer/if/then/else/function/read/write` 等，将其映射为固定类别码。
- 标识符与常数：`[a-zA-Z_][a-zA-Z0-9_]*` 识别为标识符；`[0-9]+` 识别为无符号整数常数。
- 赋值符号：`:=` 作为单一运算符（而非 `:` 与 `=` 的组合），避免额外 `=` 残留。
- 分隔符与运算符：`(`, `)`, `;`, `-`, `*`, `<`, `<=`, `>`, `>=`, `=`, `<>` 等。

参考代码：
- 记号类型定义：`include/token.h:1–33`
- 关键字表：`include/keywords.h:9–22`
- 赋值符号 `:=` 处理：`src/lexer.c:101–110`
- 词法输出：`src/lexer.c:216–229`

### 语法分析原理
- 递归下降：为每个非终结符设计一个解析函数（如 `parseExpr`, `parseTerm`, `parseFactor`, `parseStmt`, `parseDecl`, `parseBlock`），按文法规则对当前记号进行匹配。
- 行结束处理：语法分析阶段全局忽略 `EOLN`（换行）以简化跨行匹配，但保留到词法输出中以便调试。
- 顶层容错策略：将第一个 `begin` 与文件内最后一个 `end` 进行配对；若未出现 `end`，在文件末尾统一报错 `Missing 'end'`。
- 错误输出格式：统一为 `***LINE:<行号> <错误性质>`。

参考代码：
- 解析框架与函数族：`src/parse.c:88–186, 198–246, 280–332`
- if-then-else 与换行处理：`src/parse.c:216–238`
- 顶层 `begin/end` 配对与 EOF 恢复：`src/parse.c:332–362`
- 错误格式与 `match`：`src/parse.c:58–72`

### 符号表原理
- 变量项字段顺序：`vname vproc vkind vtype vlev vadr`
- 类型字符串化输出：`integer/float/char/bool`（替代数字编码）。
- 过程项包含过程名、类型（编码）、层次、变量范围（首/末地址）。

参考代码：
- 结构定义：`include/symbol.h:22–49`
- 进入/退出过程与层次维护：`src/symbol.c:55–78`
- 变量记录与类型字符串化：`src/symbol.c:26–33, 89–105`

---

## 三、实验目的
- 实现完整的编译基础流水线（词法→语法→符号输出）。
- 严格符合给定的文法与分号规则（说明表→分号→执行表）。
- 标准化错误格式输出（统一到 `.err`），实现更清晰的错误定位。
- 输出文件收敛为四类（`.dyd`, `.err`, `.var`, `.pro`）满足课程实验要求。
- 提升调试与可读性（控制台分隔日志、类型字符串化、字段顺序规范化）。

---

## 四、实验内容

### 1. 词法分析
- 识别关键字、标识符、无符号整数、运算符与分隔符。
- 输出文件：`<base>.dyd`（行格式：`<lexeme> <type>`）。
- 关键点：
  - 赋值符号 `:=` 独立识别并消费 `=` 后的下一字符，避免残留的 `=` 记号（`src/lexer.c:101–110`）。
  - 行号维护、`EOLN` 记号产生（`src/lexer.c:173–187`）。

### 2. 语法分析
- 文法范围：程序结构、说明语句、执行语句（读/写/赋值/条件语句）、表达式、函数说明与函数体。
- 全局忽略 `EOLN` 以支持跨行语法。
- 顶层 `begin/end` 配对：若无 `end`，在文件末尾统一报错。
- 错误统一写入 `<base>.err`，格式 `***LINE:<行号> <错误性质>`（例如 `Missing ';' after declaration`、`Unexpected ';' before 'else'`）。

### 3. 符号表生成
- 变量表（`<base>.var`）：`vname vproc vkind vtype vlev vadr`，类型为字符串。
- 过程表（`<base>.pro`）：过程名称、类型（编码）、层次、变量范围（首/末地址）。

---

## 五、实验器材
- 开发工具：`clang`、`make`、Trae IDE
- 编程语言：C（C17）
- 实验环境：`macOS`（Apple Silicon/M2）
- 项目结构：
  - `src/`：核心实现（lexer/parse/symbol/common）
  - `include/`：头文件（token/symbol/lexer/keywords）
  - `test/`：测试用例（`test1.mini`、`test2.mini`）
  - `output/`：结果输出（`.dyd/.err/.var/.pro`）
  - `bin/`：可执行文件（`MiniCompiler.o`）

---

## 六、实验步骤

### 1. 构建
```bash
make
```

### 2. 运行（词法→语法按主程序顺序）
```bash
./bin/MiniCompiler.o test/test1.mini output/mini_1
./bin/MiniCompiler.o test/test2.mini output/mini_2
```

### 3. 查看输出文件
- 词法输出：`output/<base>.dyd`
- 语法错误：`output/<base>.err`（无错误则为空）
- 变量表：`output/<base>.var`
- 过程表：`output/<base>.pro`

### 4. 控制台日志（分隔符）
- 词法阶段：
  - `========================================`
  - `Starting lexical analysis...`
  - `Lexical Analysis completed! Result has written to <base>.dyd`
  - `========================================`
- 语法阶段：
  - `----------------------------------------`
  - `Starting syntax analysis...`
  - `Syntax Analysis completed! Result has written to <base>.err, <base>.var, <base>.pro`
  - `----------------------------------------`

参考代码：
- 词法日志：`src/lexer.c:220–229`
- 语法日志：`src/parse.c:392–409`

---

## 七、实验数据及结果分析

### 测试程序一：`test/test1.mini`
示例（文件内容）：
```plain
begin
  integer k;
  integer function F(n);
    begin
      integer n;
      if n<=0 then F:=1
      else F:=n*F(n-1)
    end;
  read(m);
  k:=F(m);
  write(k)
end
```

- 词法输出（节选）：`output/mini_1.dyd`
  - `begin 1`, `integer 3`, `function 7`, `F 10`, `:= 20` 等
- 语法错误：`output/mini_1.err` 为空（符合文法；`if...then...else...` 内部无需分号；作为块最后一句也无需分号）
- 变量表：`output/mini_1.var`
  - `k global 0 integer 1 0`
  - `n F 1 integer 2 1`（形参）
  - `n F 0 integer 2 2`（局部）
- 过程表（节选）：`output/mini_1.pro`
  - `global ...`
  - `F ...`（包括过程层次与变量范围）

结论：完整通过，无语法错误；变量与过程记录符合预期。

### 测试程序二：`test/test2.mini`
示例（文件内容）：
```plain
begin
  integer x;
  integer y
  read(x);
  if x > 0 then
    y := x * 2;
  else
edn
```

- 语法错误：`output/mini_2.err`
  - `***LINE:4 Missing ';' after declaration`
  - `***LINE:6 Unexpected ';' before 'else'`
  - `***LINE:8 Missing ':='`
  - `***LINE:8 Invalid factor`
  - `***LINE:8 Missing 'end'`
- 变量表（类型字符串）：`output/mini_2.var`
  - `x global 0 integer 1 0`
  - `y global 0 integer 1 1`

结论：
- 第 3 行说明项缺少分号导致说明与执行表分隔不合法；
- `then` 后赋值语句末尾多分号不符合文法；
- `else` 后缺执行语句；
- 末尾 `edn` 错拼导致顶层无 `end`，末尾统一报缺失。

---

## 八、总结及心得体会
- 递归下降在小型语言子集上实现速度快、结构清晰，便于调试与维护。
- 全局忽略 `EOLN` 简化了跨行语法的处理，但错误恢复需要在语句列表与块边界设置合适的同步点，以避免级联误报。
- 错误输出统一格式显著提升定位与说明的清晰度；将输出收敛为四类文件满足课程实验的验收要求。
- 符号表类型改为字符串输出、变量表字段顺序规范化，使结果更符合教学与检查需求。

---

## 九、改进建议
- 语法扩展：支持 `+`、`/`、括号嵌套、更复杂的表达式、过程与多参数的函数。
- 语义分析：类型检查、未定义标识符检查、常量折叠、简单的中间代码或抽象语法树输出。
- 错误恢复：在语句列表、`if...else...`、块边界等位置引入同步集合，减少级联错误。
- 测试完善：设计标准化测试集（正例/反例），自动比对 `.err` 与期望。
- 输出统一：过程类型也改为字符串（`program/function/procedure`）以统一风格，便于阅读。
- 健壮性：加强输入边界检查、文件错误处理与异常路径回收。

---

## 附录 A：文法说明（子集）
```plain
<程序> → <分程序>
<分程序> → begin <说明语句表>；<执行语句表> end
<说明语句表> → <说明语句> | <说明语句表>；<说明语句>
<说明语句> → <变量说明> | <函数说明>
<变量说明> → integer <变量>
<变量> → <标识符>
<函数说明> → integer function <标识符>(<参数>)；<函数体>
<参数> → <变量>
<函数体> → begin <说明语句表>；<执行语句表> end
<执行语句表> → <执行语句> | <执行语句表>；<执行语句>
<执行语句> → <读语句> | <写语句> | <赋值语句> | <条件语句>
<读语句> → read(<变量>)
<写语句> → write(<变量>)
<赋值语句> → <变量> := <算术表达式>
<算术表达式> → <算术表达式> - <项> | <项>
<项> → <项> * <因子> | <因子>
<因子> → <变量> | <常数> | <函数调用>
<常数> → <无符号整数>
<无符号整数> → <数字> | <无符号整数><数字>
<条件语句> → if <条件表达式> then <执行语句> else <执行语句>
<条件表达式> → <算术表达式> <关系运算符> <算术表达式>
<关系运算符> → < | <= | > | >= | = | <>
```

---

## 附录 B：记号类别与编码（节选）
- 关键字：`begin(1), end(2), integer(3), if(4), then(5), else(6), function(7), read(8), write(9)`
- 标识符与常数：`identifier(10), number(11)`
- 运算符与分隔符：`=(12), <>(13), <=(14), <(15), >=(16), >(17), -(18), *(19), :=(20), ((21), )(22), ;(23)`
- 特殊：`EOLN(24), EOF(25)`

参考代码：`include/token.h:1–33`

---

## 附录 C：输出文件格式规范
- `<base>.dyd`（词法输出）
  - 每行：`<lexeme> <type>`
  - 示例：`begin 1`，`:= 20`，`k 10`
- `<base>.err`（语法错误输出）
  - 每行：`***LINE:<行号> <错误性质>`
  - 示例：`***LINE:4 Missing ';' after declaration`，`***LINE:6 Unexpected ';' before 'else'`
  - 无错误时文件为空
- `<base>.var`（变量表）
  - 字段顺序：`vname vproc vkind vtype vlev vadr`
  - 类型为字符串（如 `integer`）；`vkind` 采用编码（0 变量 / 1 形参）
  - 示例：`x global 0 integer 1 0`
- `<base>.pro`（过程表）
  - 字段：`pname ptype plev fadr ladr`（过程名、类型编码、层次、起始/结束变量地址）
  - 示例：`F 1 2 1 2`（含义视实现而定）

参考代码：
- 变量表输出：`src/symbol.c:26–33`
- 过程表输出：`src/symbol.c:40–47`

---

## 附录 D：控制台日志规范
- 词法阶段
  - 分隔符：`========================================`
  - 起始：`Starting lexical analysis...`
  - 完成：`Lexical Analysis completed! Result has written to <base>.dyd`
- 语法阶段
  - 分隔符：`----------------------------------------`
  - 起始：`Starting syntax analysis...`
  - 完成：`Syntax Analysis completed! Result has written to <base>.err, <base>.var, <base>.pro`

参考代码：
- 词法日志：`src/lexer.c:220–229`
- 语法日志：`src/parse.c:392–409`

---

## 附录 E：代码关键位置索引
- 记号类型定义：`include/token.h:1–33`
- 关键字表：`include/keywords.h:9–22`
- 赋值符号处理：`src/lexer.c:101–110`
- 词法输出（驱动）：`src/lexer.c:216–229`
- 语法错误格式与匹配：`src/parse.c:58–72`
- 全局忽略 `EOLN`：`src/parse.c:47–55`
- if-then-else 解析与分号规则：`src/parse.c:216–238`
- 顶层 begin/end 配对与 EOF 容错：`src/parse.c:332–362`
- 变量类型字符串化：`src/symbol.c:4–14`
- 变量表输出顺序与内容：`src/symbol.c:26–33`
- 过程层次维护与进入/退出：`src/symbol.c:64–66, 69–78`

---

如需将过程类型 `ptype` 改为字符串（`program/function/procedure`）以统一输出风格，或继续完善报告结构（加入状态图、语法树示例、符号表截图），请提出你的偏好，我可以基于当前项目直接补充并更新本报告。