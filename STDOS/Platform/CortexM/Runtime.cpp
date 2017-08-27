
//int assert_failed2(const char *a1, const char *a2, unsigned int a3)
//{
//  TInterrupt *v3; // r0@1

//  v3 = (TInterrupt *)SmartOS_printf("%s Line %d, %s\r\n", a1);
//  return TInterrupt::Halt(v3);
//}
//void operator delete[](void *a1)
//{
//  if ( a1 )
//    free(a1);
//}
//void operator delete[](void *a1)
//{
//  operator delete[](a1);
//}
//void operator delete(void *a1)
//{
//  if ( a1 )
//    free(a1);
//}
//void operator delete(void *a1)
//{
//  operator delete(a1);
//}
//int operator new[](unsigned int a1)
//{
//  return malloc(a1);
//}
//int operator new(unsigned int a1)
//{
//  return malloc(a1);
//}
//int _aeabi_atexit()
//{
//  return 0;
//}
//int free(void *a1)
//{
//  return Heap::Free((Heap *)Heap, a1);
//}
//int malloc(int a1)
//{
//  int v1; // r5@1
//  int v2; // r6@2
//  int v3; // r7@2
//  int result; // r0@6

//  v1 = a1;
//  if ( !Heap )
//  {
//    v2 = TSys::HeapBase((TSys *)&Sys);
//    v3 = TSys::StackTop((TSys *)&Sys);
//    if ( !(ZGV_ZZ6mallocE6g_Heap & 1) && _cxa_guard_acquire(&ZGV_ZZ6mallocE6g_Heap) )
//    {
//      Heap::Heap(&malloc::g_Heap, v2, v3 - v2);
//      _cxa_guard_release(&ZGV_ZZ6mallocE6g_Heap);
//    }
//    Heap = (int)&malloc::g_Heap;
//  }
//  result = Heap::Alloc((Heap *)Heap, v1);
//  if ( !result )
//    result = 0;
//  return result;
//}








