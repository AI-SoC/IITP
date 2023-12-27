0800013c <swap>:
swap():
main.c:13
    $display("\tTest Module: sort");
end
>*/

void swap( int* xp, int* yp )
{
 800013c:	b580      	push	{r7, lr}
 800013e:	b084      	sub	sp, #16
 8000140:	af00      	add	r7, sp, #0
 8000142:	6078      	str	r0, [r7, #4]
 8000144:	6039      	str	r1, [r7, #0]
main.c:14
    int temp = *xp;
 8000146:	687b      	ldr	r3, [r7, #4]
 8000148:	681b      	ldr	r3, [r3, #0]
 800014a:	60fb      	str	r3, [r7, #12]
main.c:15
    *xp = *yp;
 800014c:	683b      	ldr	r3, [r7, #0]
 800014e:	681a      	ldr	r2, [r3, #0]
 8000150:	687b      	ldr	r3, [r7, #4]
 8000152:	601a      	str	r2, [r3, #0]
main.c:16
    *yp = temp;
 8000154:	683b      	ldr	r3, [r7, #0]
 8000156:	68fa      	ldr	r2, [r7, #12]
 8000158:	601a      	str	r2, [r3, #0]
main.c:17
}
 800015a:	46c0      	nop			; (mov r8, r8)
 800015c:	46bd      	mov	sp, r7
 800015e:	b004      	add	sp, #16
 8000160:	bd80      	pop	{r7, pc}

08000162 <bsort>:
bsort():
main.c:20

void bsort( int *arr, int n )
{
 8000162:	b580      	push	{r7, lr}
 8000164:	b084      	sub	sp, #16
 8000166:	af00      	add	r7, sp, #0
 8000168:	6078      	str	r0, [r7, #4]
 800016a:	6039      	str	r1, [r7, #0]
main.c:22
    int i, j;
    for( i = 0; i < n - 1; i++ )
 800016c:	2300      	movs	r3, #0
 800016e:	60fb      	str	r3, [r7, #12]
 8000170:	e028      	b.n	80001c4 <bsort+0x62>
main.c:23
        for( j = 0; j < n - i - 1; j++ )
 8000172:	2300      	movs	r3, #0
 8000174:	60bb      	str	r3, [r7, #8]
 8000176:	e01b      	b.n	80001b0 <bsort+0x4e>
main.c:24
            if( arr[j] > arr[j + 1] )
 8000178:	68bb      	ldr	r3, [r7, #8]
 800017a:	009b      	lsls	r3, r3, #2
 800017c:	687a      	ldr	r2, [r7, #4]
 800017e:	18d3      	adds	r3, r2, r3
 8000180:	681a      	ldr	r2, [r3, #0]
 8000182:	68bb      	ldr	r3, [r7, #8]
 8000184:	3301      	adds	r3, #1
 8000186:	009b      	lsls	r3, r3, #2
 8000188:	6879      	ldr	r1, [r7, #4]
 800018a:	18cb      	adds	r3, r1, r3
 800018c:	681b      	ldr	r3, [r3, #0]
 800018e:	429a      	cmp	r2, r3
 8000190:	dd0b      	ble.n	80001aa <bsort+0x48>
main.c:25
                swap( &arr[j], &arr[j + 1] );
 8000192:	68bb      	ldr	r3, [r7, #8]
 8000194:	009b      	lsls	r3, r3, #2
 8000196:	687a      	ldr	r2, [r7, #4]
 8000198:	18d0      	adds	r0, r2, r3
 800019a:	68bb      	ldr	r3, [r7, #8]
 800019c:	3301      	adds	r3, #1
 800019e:	009b      	lsls	r3, r3, #2
 80001a0:	687a      	ldr	r2, [r7, #4]
 80001a2:	18d3      	adds	r3, r2, r3
 80001a4:	0019      	movs	r1, r3
 80001a6:	f7ff ffc9 	bl	800013c <swap>
main.c:23 (discriminator 2)
        for( j = 0; j < n - i - 1; j++ )
 80001aa:	68bb      	ldr	r3, [r7, #8]
 80001ac:	3301      	adds	r3, #1
 80001ae:	60bb      	str	r3, [r7, #8]
main.c:23 (discriminator 1)
 80001b0:	683a      	ldr	r2, [r7, #0]
 80001b2:	68fb      	ldr	r3, [r7, #12]
 80001b4:	1ad3      	subs	r3, r2, r3
 80001b6:	3b01      	subs	r3, #1
 80001b8:	68ba      	ldr	r2, [r7, #8]
 80001ba:	429a      	cmp	r2, r3
 80001bc:	dbdc      	blt.n	8000178 <bsort+0x16>
main.c:22 (discriminator 2)
    for( i = 0; i < n - 1; i++ )
 80001be:	68fb      	ldr	r3, [r7, #12]
 80001c0:	3301      	adds	r3, #1
 80001c2:	60fb      	str	r3, [r7, #12]
main.c:22 (discriminator 1)
 80001c4:	683b      	ldr	r3, [r7, #0]
 80001c6:	3b01      	subs	r3, #1
 80001c8:	68fa      	ldr	r2, [r7, #12]
 80001ca:	429a      	cmp	r2, r3
 80001cc:	dbd1      	blt.n	8000172 <bsort+0x10>
main.c:26
}
 80001ce:	46c0      	nop			; (mov r8, r8)
 80001d0:	46c0      	nop			; (mov r8, r8)
 80001d2:	46bd      	mov	sp, r7
 80001d4:	b004      	add	sp, #16
 80001d6:	bd80      	pop	{r7, pc}

080001d8 <ssort>:
ssort():
main.c:29

void ssort( int *arr, int n)
{
 80001d8:	b580      	push	{r7, lr}
 80001da:	b086      	sub	sp, #24
 80001dc:	af00      	add	r7, sp, #0
 80001de:	6078      	str	r0, [r7, #4]
 80001e0:	6039      	str	r1, [r7, #0]
main.c:31
	int i, j, tmp;
	for( i = 0; i < n - 1; i++ )
 80001e2:	2300      	movs	r3, #0
 80001e4:	617b      	str	r3, [r7, #20]
 80001e6:	e028      	b.n	800023a <ssort+0x62>
main.c:33
	{
		tmp = i;
 80001e8:	697b      	ldr	r3, [r7, #20]
 80001ea:	60fb      	str	r3, [r7, #12]
main.c:34
		for( j = i+1; j < n; j++ )
 80001ec:	697b      	ldr	r3, [r7, #20]
 80001ee:	3301      	adds	r3, #1
 80001f0:	613b      	str	r3, [r7, #16]
 80001f2:	e010      	b.n	8000216 <ssort+0x3e>
main.c:35
			if( arr[tmp] >= arr[j] ) tmp = j;
 80001f4:	68fb      	ldr	r3, [r7, #12]
 80001f6:	009b      	lsls	r3, r3, #2
 80001f8:	687a      	ldr	r2, [r7, #4]
 80001fa:	18d3      	adds	r3, r2, r3
 80001fc:	681a      	ldr	r2, [r3, #0]
 80001fe:	693b      	ldr	r3, [r7, #16]
 8000200:	009b      	lsls	r3, r3, #2
 8000202:	6879      	ldr	r1, [r7, #4]
 8000204:	18cb      	adds	r3, r1, r3
 8000206:	681b      	ldr	r3, [r3, #0]
 8000208:	429a      	cmp	r2, r3
 800020a:	db01      	blt.n	8000210 <ssort+0x38>
main.c:35 (discriminator 1)
 800020c:	693b      	ldr	r3, [r7, #16]
 800020e:	60fb      	str	r3, [r7, #12]
main.c:34 (discriminator 2)
		for( j = i+1; j < n; j++ )
 8000210:	693b      	ldr	r3, [r7, #16]
 8000212:	3301      	adds	r3, #1
 8000214:	613b      	str	r3, [r7, #16]
main.c:34 (discriminator 1)
 8000216:	693a      	ldr	r2, [r7, #16]
 8000218:	683b      	ldr	r3, [r7, #0]
 800021a:	429a      	cmp	r2, r3
 800021c:	dbea      	blt.n	80001f4 <ssort+0x1c>
main.c:36 (discriminator 2)
		swap( &arr[j], &arr[tmp] );
 800021e:	693b      	ldr	r3, [r7, #16]
 8000220:	009b      	lsls	r3, r3, #2
 8000222:	687a      	ldr	r2, [r7, #4]
 8000224:	18d0      	adds	r0, r2, r3
 8000226:	68fb      	ldr	r3, [r7, #12]
 8000228:	009b      	lsls	r3, r3, #2
 800022a:	687a      	ldr	r2, [r7, #4]
 800022c:	18d3      	adds	r3, r2, r3
 800022e:	0019      	movs	r1, r3
 8000230:	f7ff ff84 	bl	800013c <swap>
main.c:31 (discriminator 2)
	for( i = 0; i < n - 1; i++ )
 8000234:	697b      	ldr	r3, [r7, #20]
 8000236:	3301      	adds	r3, #1
 8000238:	617b      	str	r3, [r7, #20]
main.c:31 (discriminator 1)
 800023a:	683b      	ldr	r3, [r7, #0]
 800023c:	3b01      	subs	r3, #1
 800023e:	697a      	ldr	r2, [r7, #20]
 8000240:	429a      	cmp	r2, r3
 8000242:	dbd1      	blt.n	80001e8 <ssort+0x10>
main.c:38
	}
}
 8000244:	46c0      	nop			; (mov r8, r8)
 8000246:	46c0      	nop			; (mov r8, r8)
 8000248:	46bd      	mov	sp, r7
 800024a:	b006      	add	sp, #24
 800024c:	bd80      	pop	{r7, pc}

0800024e <qsort>:
qsort():
main.c:40

void qsort( int *arr, int l, int r ) {
 800024e:	b580      	push	{r7, lr}
 8000250:	b088      	sub	sp, #32
 8000252:	af00      	add	r7, sp, #0
 8000254:	60f8      	str	r0, [r7, #12]
 8000256:	60b9      	str	r1, [r7, #8]
 8000258:	607a      	str	r2, [r7, #4]
main.c:41
	int left = l, right = r;
 800025a:	68bb      	ldr	r3, [r7, #8]
 800025c:	61fb      	str	r3, [r7, #28]
 800025e:	687b      	ldr	r3, [r7, #4]
 8000260:	61bb      	str	r3, [r7, #24]
main.c:42
	int pivot = arr[(l+r)/2];
 8000262:	68ba      	ldr	r2, [r7, #8]
 8000264:	687b      	ldr	r3, [r7, #4]
 8000266:	18d3      	adds	r3, r2, r3
 8000268:	2b00      	cmp	r3, #0
 800026a:	da00      	bge.n	800026e <qsort+0x20>
 800026c:	3301      	adds	r3, #1
 800026e:	105b      	asrs	r3, r3, #1
 8000270:	009b      	lsls	r3, r3, #2
 8000272:	68fa      	ldr	r2, [r7, #12]
 8000274:	18d3      	adds	r3, r2, r3
 8000276:	681b      	ldr	r3, [r3, #0]
 8000278:	617b      	str	r3, [r7, #20]
main.c:44

	while( left <= right ) {
 800027a:	e02b      	b.n	80002d4 <qsort+0x86>
main.c:45 (discriminator 2)
		while( arr[left] < pivot ) left++;
 800027c:	69fb      	ldr	r3, [r7, #28]
 800027e:	3301      	adds	r3, #1
 8000280:	61fb      	str	r3, [r7, #28]
main.c:45 (discriminator 1)
 8000282:	69fb      	ldr	r3, [r7, #28]
 8000284:	009b      	lsls	r3, r3, #2
 8000286:	68fa      	ldr	r2, [r7, #12]
 8000288:	18d3      	adds	r3, r2, r3
 800028a:	681b      	ldr	r3, [r3, #0]
 800028c:	697a      	ldr	r2, [r7, #20]
 800028e:	429a      	cmp	r2, r3
 8000290:	dcf4      	bgt.n	800027c <qsort+0x2e>
main.c:46
		while( arr[right] > pivot ) right--;
 8000292:	e002      	b.n	800029a <qsort+0x4c>
main.c:46 (discriminator 2)
 8000294:	69bb      	ldr	r3, [r7, #24]
 8000296:	3b01      	subs	r3, #1
 8000298:	61bb      	str	r3, [r7, #24]
main.c:46 (discriminator 1)
 800029a:	69bb      	ldr	r3, [r7, #24]
 800029c:	009b      	lsls	r3, r3, #2
 800029e:	68fa      	ldr	r2, [r7, #12]
 80002a0:	18d3      	adds	r3, r2, r3
 80002a2:	681b      	ldr	r3, [r3, #0]
 80002a4:	697a      	ldr	r2, [r7, #20]
 80002a6:	429a      	cmp	r2, r3
 80002a8:	dbf4      	blt.n	8000294 <qsort+0x46>
main.c:48

		if( left <= right ) {
 80002aa:	69fa      	ldr	r2, [r7, #28]
 80002ac:	69bb      	ldr	r3, [r7, #24]
 80002ae:	429a      	cmp	r2, r3
 80002b0:	dc10      	bgt.n	80002d4 <qsort+0x86>
main.c:49
			swap( &arr[left], &arr[right] );
 80002b2:	69fb      	ldr	r3, [r7, #28]
 80002b4:	009b      	lsls	r3, r3, #2
 80002b6:	68fa      	ldr	r2, [r7, #12]
 80002b8:	18d0      	adds	r0, r2, r3
 80002ba:	69bb      	ldr	r3, [r7, #24]
 80002bc:	009b      	lsls	r3, r3, #2
 80002be:	68fa      	ldr	r2, [r7, #12]
 80002c0:	18d3      	adds	r3, r2, r3
 80002c2:	0019      	movs	r1, r3
 80002c4:	f7ff ff3a 	bl	800013c <swap>
main.c:50
			left++;
 80002c8:	69fb      	ldr	r3, [r7, #28]
 80002ca:	3301      	adds	r3, #1
 80002cc:	61fb      	str	r3, [r7, #28]
main.c:51
			right--;
 80002ce:	69bb      	ldr	r3, [r7, #24]
 80002d0:	3b01      	subs	r3, #1
 80002d2:	61bb      	str	r3, [r7, #24]
main.c:44
	while( left <= right ) {
 80002d4:	69fa      	ldr	r2, [r7, #28]
 80002d6:	69bb      	ldr	r3, [r7, #24]
 80002d8:	429a      	cmp	r2, r3
 80002da:	ddd2      	ble.n	8000282 <qsort+0x34>
main.c:54
		}
	}
	if( l < right ) qsort( arr, l, right );
 80002dc:	68ba      	ldr	r2, [r7, #8]
 80002de:	69bb      	ldr	r3, [r7, #24]
 80002e0:	429a      	cmp	r2, r3
 80002e2:	da05      	bge.n	80002f0 <qsort+0xa2>
main.c:54 (discriminator 1)
 80002e4:	69ba      	ldr	r2, [r7, #24]
 80002e6:	68b9      	ldr	r1, [r7, #8]
 80002e8:	68fb      	ldr	r3, [r7, #12]
 80002ea:	0018      	movs	r0, r3
 80002ec:	f7ff ffaf 	bl	800024e <qsort>
main.c:55
	if( r > left ) qsort( arr, left, r );
 80002f0:	687a      	ldr	r2, [r7, #4]
 80002f2:	69fb      	ldr	r3, [r7, #28]
 80002f4:	429a      	cmp	r2, r3
 80002f6:	dd05      	ble.n	8000304 <qsort+0xb6>
main.c:55 (discriminator 1)
 80002f8:	687a      	ldr	r2, [r7, #4]
 80002fa:	69f9      	ldr	r1, [r7, #28]
 80002fc:	68fb      	ldr	r3, [r7, #12]
 80002fe:	0018      	movs	r0, r3
 8000300:	f7ff ffa5 	bl	800024e <qsort>
main.c:56
}
 8000304:	46c0      	nop			; (mov r8, r8)
 8000306:	46bd      	mov	sp, r7
 8000308:	b008      	add	sp, #32
 800030a:	bd80      	pop	{r7, pc}

0800030c <heapify>:
heapify():
main.c:59

void heapify( int *arr, int here, int size )
{
 800030c:	b580      	push	{r7, lr}
 800030e:	b088      	sub	sp, #32
 8000310:	af00      	add	r7, sp, #0
 8000312:	60f8      	str	r0, [r7, #12]
 8000314:	60b9      	str	r1, [r7, #8]
 8000316:	607a      	str	r2, [r7, #4]
main.c:60
	int left = here * 2 + 1;
 8000318:	68bb      	ldr	r3, [r7, #8]
 800031a:	005b      	lsls	r3, r3, #1
 800031c:	3301      	adds	r3, #1
 800031e:	61bb      	str	r3, [r7, #24]
main.c:61
	int right = here * 2 + 2;
 8000320:	68bb      	ldr	r3, [r7, #8]
 8000322:	3301      	adds	r3, #1
 8000324:	005b      	lsls	r3, r3, #1
 8000326:	617b      	str	r3, [r7, #20]
main.c:62
	int max = here;
 8000328:	68bb      	ldr	r3, [r7, #8]
 800032a:	61fb      	str	r3, [r7, #28]
main.c:63
	if( left < size && arr[left] > arr[max] )
 800032c:	69ba      	ldr	r2, [r7, #24]
 800032e:	687b      	ldr	r3, [r7, #4]
 8000330:	429a      	cmp	r2, r3
 8000332:	da0d      	bge.n	8000350 <heapify+0x44>
main.c:63 (discriminator 1)
 8000334:	69bb      	ldr	r3, [r7, #24]
 8000336:	009b      	lsls	r3, r3, #2
 8000338:	68fa      	ldr	r2, [r7, #12]
 800033a:	18d3      	adds	r3, r2, r3
 800033c:	681a      	ldr	r2, [r3, #0]
 800033e:	69fb      	ldr	r3, [r7, #28]
 8000340:	009b      	lsls	r3, r3, #2
 8000342:	68f9      	ldr	r1, [r7, #12]
 8000344:	18cb      	adds	r3, r1, r3
 8000346:	681b      	ldr	r3, [r3, #0]
 8000348:	429a      	cmp	r2, r3
 800034a:	dd01      	ble.n	8000350 <heapify+0x44>
main.c:64
		max = left;
 800034c:	69bb      	ldr	r3, [r7, #24]
 800034e:	61fb      	str	r3, [r7, #28]
main.c:65
	if( right < size && arr[right] > arr[max] )
 8000350:	697a      	ldr	r2, [r7, #20]
 8000352:	687b      	ldr	r3, [r7, #4]
 8000354:	429a      	cmp	r2, r3
 8000356:	da0d      	bge.n	8000374 <heapify+0x68>
main.c:65 (discriminator 1)
 8000358:	697b      	ldr	r3, [r7, #20]
 800035a:	009b      	lsls	r3, r3, #2
 800035c:	68fa      	ldr	r2, [r7, #12]
 800035e:	18d3      	adds	r3, r2, r3
 8000360:	681a      	ldr	r2, [r3, #0]
 8000362:	69fb      	ldr	r3, [r7, #28]
 8000364:	009b      	lsls	r3, r3, #2
 8000366:	68f9      	ldr	r1, [r7, #12]
 8000368:	18cb      	adds	r3, r1, r3
 800036a:	681b      	ldr	r3, [r3, #0]
 800036c:	429a      	cmp	r2, r3
 800036e:	dd01      	ble.n	8000374 <heapify+0x68>
main.c:66
		max = right;
 8000370:	697b      	ldr	r3, [r7, #20]
 8000372:	61fb      	str	r3, [r7, #28]
main.c:67
	if( max != here ) {
 8000374:	69fa      	ldr	r2, [r7, #28]
 8000376:	68bb      	ldr	r3, [r7, #8]
 8000378:	429a      	cmp	r2, r3
 800037a:	d010      	beq.n	800039e <heapify+0x92>
main.c:68
		swap( &arr[here], &arr[max] );
 800037c:	68bb      	ldr	r3, [r7, #8]
 800037e:	009b      	lsls	r3, r3, #2
 8000380:	68fa      	ldr	r2, [r7, #12]
 8000382:	18d0      	adds	r0, r2, r3
 8000384:	69fb      	ldr	r3, [r7, #28]
 8000386:	009b      	lsls	r3, r3, #2
 8000388:	68fa      	ldr	r2, [r7, #12]
 800038a:	18d3      	adds	r3, r2, r3
 800038c:	0019      	movs	r1, r3
 800038e:	f7ff fed5 	bl	800013c <swap>
main.c:69
		heapify( arr, max, size );
 8000392:	687a      	ldr	r2, [r7, #4]
 8000394:	69f9      	ldr	r1, [r7, #28]
 8000396:	68fb      	ldr	r3, [r7, #12]
 8000398:	0018      	movs	r0, r3
 800039a:	f7ff ffb7 	bl	800030c <heapify>
main.c:71
	}
}
 800039e:	46c0      	nop			; (mov r8, r8)
 80003a0:	46bd      	mov	sp, r7
 80003a2:	b008      	add	sp, #32
 80003a4:	bd80      	pop	{r7, pc}

080003a6 <buildHeap>:
buildHeap():
main.c:74

void buildHeap( int *arr, int size )
{
 80003a6:	b580      	push	{r7, lr}
 80003a8:	b084      	sub	sp, #16
 80003aa:	af00      	add	r7, sp, #0
 80003ac:	6078      	str	r0, [r7, #4]
 80003ae:	6039      	str	r1, [r7, #0]
main.c:76
	int i, j;
	for( i = size / 2 - 1; i >= 0; i-- )
 80003b0:	683b      	ldr	r3, [r7, #0]
 80003b2:	2b00      	cmp	r3, #0
 80003b4:	da00      	bge.n	80003b8 <buildHeap+0x12>
 80003b6:	3301      	adds	r3, #1
 80003b8:	105b      	asrs	r3, r3, #1
 80003ba:	3b01      	subs	r3, #1
 80003bc:	60fb      	str	r3, [r7, #12]
 80003be:	e008      	b.n	80003d2 <buildHeap+0x2c>
main.c:77 (discriminator 3)
		heapify( arr, i, size );
 80003c0:	683a      	ldr	r2, [r7, #0]
 80003c2:	68f9      	ldr	r1, [r7, #12]
 80003c4:	687b      	ldr	r3, [r7, #4]
 80003c6:	0018      	movs	r0, r3
 80003c8:	f7ff ffa0 	bl	800030c <heapify>
main.c:76 (discriminator 3)
	for( i = size / 2 - 1; i >= 0; i-- )
 80003cc:	68fb      	ldr	r3, [r7, #12]
 80003ce:	3b01      	subs	r3, #1
 80003d0:	60fb      	str	r3, [r7, #12]
main.c:76 (discriminator 1)
 80003d2:	68fb      	ldr	r3, [r7, #12]
 80003d4:	2b00      	cmp	r3, #0
 80003d6:	daf3      	bge.n	80003c0 <buildHeap+0x1a>
main.c:78
}
 80003d8:	46c0      	nop			; (mov r8, r8)
 80003da:	46c0      	nop			; (mov r8, r8)
 80003dc:	46bd      	mov	sp, r7
 80003de:	b004      	add	sp, #16
 80003e0:	bd80      	pop	{r7, pc}

080003e2 <hsort>:
hsort():
main.c:80

void hsort( int *arr, int size ) {
 80003e2:	b580      	push	{r7, lr}
 80003e4:	b084      	sub	sp, #16
 80003e6:	af00      	add	r7, sp, #0
 80003e8:	6078      	str	r0, [r7, #4]
 80003ea:	6039      	str	r1, [r7, #0]
main.c:82
	int treeSize;
	buildHeap( arr, size );
 80003ec:	683a      	ldr	r2, [r7, #0]
 80003ee:	687b      	ldr	r3, [r7, #4]
 80003f0:	0011      	movs	r1, r2
 80003f2:	0018      	movs	r0, r3
 80003f4:	f7ff ffd7 	bl	80003a6 <buildHeap>
main.c:83
	for( treeSize = size - 1; treeSize >= 0; treeSize-- )
 80003f8:	683b      	ldr	r3, [r7, #0]
 80003fa:	3b01      	subs	r3, #1
 80003fc:	60fb      	str	r3, [r7, #12]
 80003fe:	e011      	b.n	8000424 <hsort+0x42>
main.c:85 (discriminator 3)
	{
		swap( &arr[0], &arr[treeSize] );
 8000400:	68fb      	ldr	r3, [r7, #12]
 8000402:	009b      	lsls	r3, r3, #2
 8000404:	687a      	ldr	r2, [r7, #4]
 8000406:	18d2      	adds	r2, r2, r3
 8000408:	687b      	ldr	r3, [r7, #4]
 800040a:	0011      	movs	r1, r2
 800040c:	0018      	movs	r0, r3
 800040e:	f7ff fe95 	bl	800013c <swap>
main.c:86 (discriminator 3)
		heapify( arr, 0, treeSize );
 8000412:	68fa      	ldr	r2, [r7, #12]
 8000414:	687b      	ldr	r3, [r7, #4]
 8000416:	2100      	movs	r1, #0
 8000418:	0018      	movs	r0, r3
 800041a:	f7ff ff77 	bl	800030c <heapify>
main.c:83 (discriminator 3)
	for( treeSize = size - 1; treeSize >= 0; treeSize-- )
 800041e:	68fb      	ldr	r3, [r7, #12]
 8000420:	3b01      	subs	r3, #1
 8000422:	60fb      	str	r3, [r7, #12]
main.c:83 (discriminator 1)
 8000424:	68fb      	ldr	r3, [r7, #12]
 8000426:	2b00      	cmp	r3, #0
 8000428:	daea      	bge.n	8000400 <hsort+0x1e>
main.c:88
	}
}
 800042a:	46c0      	nop			; (mov r8, r8)
 800042c:	46c0      	nop			; (mov r8, r8)
 800042e:	46bd      	mov	sp, r7
 8000430:	b004      	add	sp, #16
 8000432:	bd80      	pop	{r7, pc}

08000434 <main>:
main():
main.c:91

int main( void )
{
 8000434:	b5f0      	push	{r4, r5, r6, r7, lr}
 8000436:	b0af      	sub	sp, #188	; 0xbc
 8000438:	af00      	add	r7, sp, #0
main.c:95
    __asm(".syntax unified\n\t");

	int n;
    int arr1[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
 800043a:	2188      	movs	r1, #136	; 0x88
 800043c:	187b      	adds	r3, r7, r1
 800043e:	4a2e      	ldr	r2, [pc, #184]	; (80004f8 <main+0xc4>)
 8000440:	ca31      	ldmia	r2!, {r0, r4, r5}
 8000442:	c331      	stmia	r3!, {r0, r4, r5}
 8000444:	ca31      	ldmia	r2!, {r0, r4, r5}
 8000446:	c331      	stmia	r3!, {r0, r4, r5}
 8000448:	ca31      	ldmia	r2!, {r0, r4, r5}
 800044a:	c331      	stmia	r3!, {r0, r4, r5}
 800044c:	ca11      	ldmia	r2!, {r0, r4}
 800044e:	c311      	stmia	r3!, {r0, r4}
main.c:96
	n = sizeof( arr1 ) / sizeof( arr1[0] ); 
 8000450:	230b      	movs	r3, #11
 8000452:	24b4      	movs	r4, #180	; 0xb4
 8000454:	193a      	adds	r2, r7, r4
 8000456:	6013      	str	r3, [r2, #0]
main.c:97
    bsort( arr1, n );
 8000458:	193b      	adds	r3, r7, r4
 800045a:	681a      	ldr	r2, [r3, #0]
 800045c:	187b      	adds	r3, r7, r1
 800045e:	0011      	movs	r1, r2
 8000460:	0018      	movs	r0, r3
 8000462:	f7ff fe7e 	bl	8000162 <bsort>
main.c:98
    __asm("NOP\n\t");
 8000466:	46c0      	nop			; (mov r8, r8)
main.c:102
    //{
    //    $display(" 1. Bubble sort started!!");
    //}
    __asm("NOP\n\t");
 8000468:	46c0      	nop			; (mov r8, r8)
main.c:110
        force `REG.r_R0 = 32'h0000;
    `endif
    }*/


    int arr2[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
 800046a:	215c      	movs	r1, #92	; 0x5c
 800046c:	187b      	adds	r3, r7, r1
 800046e:	4a22      	ldr	r2, [pc, #136]	; (80004f8 <main+0xc4>)
 8000470:	ca61      	ldmia	r2!, {r0, r5, r6}
 8000472:	c361      	stmia	r3!, {r0, r5, r6}
 8000474:	ca61      	ldmia	r2!, {r0, r5, r6}
 8000476:	c361      	stmia	r3!, {r0, r5, r6}
 8000478:	ca61      	ldmia	r2!, {r0, r5, r6}
 800047a:	c361      	stmia	r3!, {r0, r5, r6}
 800047c:	ca21      	ldmia	r2!, {r0, r5}
 800047e:	c321      	stmia	r3!, {r0, r5}
main.c:111
	n = sizeof( arr2 ) / sizeof( arr2[0] );
 8000480:	230b      	movs	r3, #11
 8000482:	193a      	adds	r2, r7, r4
 8000484:	6013      	str	r3, [r2, #0]
main.c:112
	ssort( arr2, n );
 8000486:	193b      	adds	r3, r7, r4
 8000488:	681a      	ldr	r2, [r3, #0]
 800048a:	187b      	adds	r3, r7, r1
 800048c:	0011      	movs	r1, r2
 800048e:	0018      	movs	r0, r3
 8000490:	f7ff fea2 	bl	80001d8 <ssort>
main.c:113
    __asm("NOP\n\t");
 8000494:	46c0      	nop			; (mov r8, r8)
main.c:119
    //{
    //    $display(" 2. Selection sort started!!");
    //}


    int arr3[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
 8000496:	2130      	movs	r1, #48	; 0x30
 8000498:	187b      	adds	r3, r7, r1
 800049a:	4a17      	ldr	r2, [pc, #92]	; (80004f8 <main+0xc4>)
 800049c:	ca61      	ldmia	r2!, {r0, r5, r6}
 800049e:	c361      	stmia	r3!, {r0, r5, r6}
 80004a0:	ca61      	ldmia	r2!, {r0, r5, r6}
 80004a2:	c361      	stmia	r3!, {r0, r5, r6}
 80004a4:	ca61      	ldmia	r2!, {r0, r5, r6}
 80004a6:	c361      	stmia	r3!, {r0, r5, r6}
 80004a8:	ca21      	ldmia	r2!, {r0, r5}
 80004aa:	c321      	stmia	r3!, {r0, r5}
main.c:120
	n = sizeof( arr3 ) / sizeof( arr3[0] );
 80004ac:	230b      	movs	r3, #11
 80004ae:	193a      	adds	r2, r7, r4
 80004b0:	6013      	str	r3, [r2, #0]
main.c:121
	qsort( arr3, 0, n-1 );
 80004b2:	193b      	adds	r3, r7, r4
 80004b4:	681b      	ldr	r3, [r3, #0]
 80004b6:	1e5a      	subs	r2, r3, #1
 80004b8:	187b      	adds	r3, r7, r1
 80004ba:	2100      	movs	r1, #0
 80004bc:	0018      	movs	r0, r3
 80004be:	f7ff fec6 	bl	800024e <qsort>
main.c:122
	__asm("NOP\n\t");
 80004c2:	46c0      	nop			; (mov r8, r8)
main.c:128
    //{
    //    $display(" 3. Quick sort started!!");
    //}


    int arr4[] = { 5, 1, 4, 2, 8, 10, 3, 15, 7, 20, 35 };
 80004c4:	1d3b      	adds	r3, r7, #4
 80004c6:	4a0c      	ldr	r2, [pc, #48]	; (80004f8 <main+0xc4>)
 80004c8:	ca23      	ldmia	r2!, {r0, r1, r5}
 80004ca:	c323      	stmia	r3!, {r0, r1, r5}
 80004cc:	ca23      	ldmia	r2!, {r0, r1, r5}
 80004ce:	c323      	stmia	r3!, {r0, r1, r5}
 80004d0:	ca23      	ldmia	r2!, {r0, r1, r5}
 80004d2:	c323      	stmia	r3!, {r0, r1, r5}
 80004d4:	ca03      	ldmia	r2!, {r0, r1}
 80004d6:	c303      	stmia	r3!, {r0, r1}
main.c:129
	n = sizeof( arr4 ) / sizeof( arr4[0] );
 80004d8:	230b      	movs	r3, #11
 80004da:	193a      	adds	r2, r7, r4
 80004dc:	6013      	str	r3, [r2, #0]
main.c:130
	hsort( arr4, n );
 80004de:	193b      	adds	r3, r7, r4
 80004e0:	681a      	ldr	r2, [r3, #0]
 80004e2:	1d3b      	adds	r3, r7, #4
 80004e4:	0011      	movs	r1, r2
 80004e6:	0018      	movs	r0, r3
 80004e8:	f7ff ff7b 	bl	80003e2 <hsort>
main.c:131
    __asm("NOP\n\t");
 80004ec:	46c0      	nop			; (mov r8, r8)
main.c:136
    //{
    //    $display( " 4. Heap sort started!!");
    //}

	return 0;
 80004ee:	2300      	movs	r3, #0
main.c:137
}
 80004f0:	0018      	movs	r0, r3
 80004f2:	46bd      	mov	sp, r7
 80004f4:	b02f      	add	sp, #188	; 0xbc
 80004f6:	bdf0      	pop	{r4, r5, r6, r7, pc}
 80004f8:	080004fc 	stmdaeq	r0, {r2, r3, r4, r5, r6, r7, sl}
 80004fc:	00000005 	andeq	r0, r0, r5
 8000500:	00000001 	andeq	r0, r0, r1
 8000504:	00000004 	andeq	r0, r0, r4
 8000508:	00000002 	andeq	r0, r0, r2
 800050c:	00000008 	andeq	r0, r0, r8
 8000510:	0000000a 	andeq	r0, r0, sl
 8000514:	00000003 	andeq	r0, r0, r3
 8000518:	0000000f 	andeq	r0, r0, pc
 800051c:	00000007 	andeq	r0, r0, r7
 8000520:	00000014 	andeq	r0, r0, r4, lsl r0
 8000524:	00000023 	andeq	r0, r0, r3, lsr #32
