
out.elf:     file format elf32-littlearm


Disassembly of section .init:

0800018c <_init>:
 800018c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800018e:	bf00      	nop

Disassembly of section .fini:

08000190 <_fini>:
 8000190:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8000192:	bf00      	nop

Disassembly of section .text.main:

08000194 <main>:

int main(){
 8000194:	b480      	push	{r7}
 8000196:	b085      	sub	sp, #20
 8000198:	af00      	add	r7, sp, #0
    int i;
    int num1,num2,num3;
    num1=1;
 800019a:	2301      	movs	r3, #1
 800019c:	60bb      	str	r3, [r7, #8]
    num2=1;
 800019e:	2301      	movs	r3, #1
 80001a0:	607b      	str	r3, [r7, #4]

    for(i=0;i<6;i++){
 80001a2:	2300      	movs	r3, #0
 80001a4:	60fb      	str	r3, [r7, #12]
 80001a6:	e00a      	b.n	80001be <main+0x2a>
         num3=num2+num1;
 80001a8:	687a      	ldr	r2, [r7, #4]
 80001aa:	68bb      	ldr	r3, [r7, #8]
 80001ac:	4413      	add	r3, r2
 80001ae:	603b      	str	r3, [r7, #0]
         num2=num1;
 80001b0:	68bb      	ldr	r3, [r7, #8]
 80001b2:	607b      	str	r3, [r7, #4]
         num1=num3;
 80001b4:	683b      	ldr	r3, [r7, #0]
 80001b6:	60bb      	str	r3, [r7, #8]
    for(i=0;i<6;i++){
 80001b8:	68fb      	ldr	r3, [r7, #12]
 80001ba:	3301      	adds	r3, #1
 80001bc:	60fb      	str	r3, [r7, #12]
 80001be:	68fb      	ldr	r3, [r7, #12]
 80001c0:	2b05      	cmp	r3, #5
 80001c2:	ddf1      	ble.n	80001a8 <main+0x14>
 80001c4:	2300      	movs	r3, #0
    }
}
 80001c6:	4618      	mov	r0, r3
 80001c8:	3714      	adds	r7, #20
 80001ca:	46bd      	mov	sp, r7
 80001cc:	f85d 7b04 	ldr.w	r7, [sp], #4
 80001d0:	4770      	bx	lr
