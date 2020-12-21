## jz, je

`CMP` **subtracts** the operands and sets the flags. Namely, it sets the zero flag if the difference is zero (operands are equal).

`TEST` sets the zero flag, `ZF`, when the result of the **AND** operation is zero. If two operands are equal, their bitwise AND is zero when both are zero. `TEST` also sets the sign flag, `SF`, when the most significant bit is set in the result, and the parity flag, `PF`, when the number of set bits is even.

`JE` [Jump if Equals] tests the zero flag and jumps if the flag is set. `JE` is an alias of `JZ` [Jump if Zero] so the disassembler cannot select one based on the opcode. `JE` is named such because the zero flag is set if the arguments to `CMP` are equal.

So,

```assembly
TEST %eax, %eax
JE   400e77 <phase_1+0x23>
```

jumps if the `%eax` is zero.

## Conditional Jump

| Instruction | Description                                                | signed-ness | Flags              | short jump opcodes | near jump opcodes |
| ----------- | ---------------------------------------------------------- | ----------- | ------------------ | ------------------ | ----------------- |
| JO          | Jump if overflow                                           |             | OF = 1             | 70                 | 0F 80             |
| JNO         | Jump if not overflow                                       |             | OF = 0             | 71                 | 0F 81             |
| JS          | Jump if sign                                               |             | SF = 1             | 78                 | 0F 88             |
| JNS         | Jump if not sign                                           |             | SF = 0             | 79                 | 0F 89             |
| JE JZ       | Jump if equal Jump if zero                                 |             | ZF = 1             | 74                 | 0F 84             |
| JNE JNZ     | Jump if not equal Jump if not zero                         |             | ZF = 0             | 75                 | 0F 85             |
| JB JNAE JC  | Jump if below Jump if not above or equal Jump if carry     | unsigned    | CF = 1             | 72                 | 0F 82             |
| JNB JAE JNC | Jump if not below Jump if above or equal Jump if not carry | unsigned    | CF = 0             | 73                 | 0F 83             |
| JBE JNA     | Jump if below or equal Jump if not above                   | unsigned    | CF = 1 or ZF = 1   | 76                 | 0F 86             |
| JA JNBE     | Jump if above Jump if not below or equal                   | unsigned    | CF = 0 and ZF = 0  | 77                 | 0F 87             |
| JL JNGE     | Jump if less Jump if not greater or equal                  | signed      | SF <> OF           | 7C                 | 0F 8C             |
| JGE JNL     | Jump if greater or equal Jump if not less                  | signed      | SF = OF            | 7D                 | 0F 8D             |
| JLE JNG     | Jump if less or equal Jump if not greater                  | signed      | ZF = 1 or SF <> OF | 7E                 | 0F 8E             |
| JG JNLE     | Jump if greater Jump if not less or equal                  | signed      | ZF = 0 and SF = OF | 7F                 | 0F 8F             |
| JP JPE      | Jump if parity Jump if parity even                         |             | PF = 1             | 7A                 | 0F 8A             |
| JNP JPO     | Jump if not parity Jump if parity odd                      |             | PF = 0             | 7B                 | 0F 8B             |
| JCXZ JECXZ  | Jump if %CX register is 0 Jump if %ECX register is 0       |             | %CX = 0 %ECX = 0   | E3                 |                   |

### Processor Flags

CF - carry flag

Set on high-order bit carry or borrow; cleared otherwise

PF - parity flag

Set if low-order eight bits of result contain an even number of "1" bits; cleared otherwise

ZF - zero flags

Set if result is zero; cleared otherwise

SF - sign flag

Set equal to high-order bit of result (0 if positive 1 if negative)

OF - overflow flag

Set if result is too large a positive number or too small a negative number (excluding sign bit) to fit in destination operand; cleared otherwise

## movzx, movsxd

mov zero extend

mov sign entend double word

## CBW、CWD、CDQ、CWDE、CDQE

cbw 使用al的最高位拓展ah的所有位

cwd使用ax的最高位拓展dx的所有位

cdq使用eax的最高位拓展edx的所有位

cwde使用ax的最高位拓展eax高16位的所有位

cdqe使用eax的最高位拓展rax高32位的所有位