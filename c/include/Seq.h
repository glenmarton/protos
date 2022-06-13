/* $Id: seq.h 6 2007-01-22 00:45:22Z drhanson $ */
#ifndef SEQ_INCLUDED
#define SEQ_INCLUDED
#define T Seq_T
typedef struct seq_t *T;
extern T Seq_new(size_t hint);
extern T Seq_seq(void *x, ...);
extern void Seq_free(T *seq);
extern size_t Seq_length(T seq);
extern void *Seq_get(T seq, size_t i);
extern void *Seq_put(T seq, size_t i, void *x);
extern void *Seq_addlo(T seq, void *x);
extern void *Seq_addhi(T seq, void *x);
extern void *Seq_remlo(T seq);
extern void *Seq_remhi(T seq);
#undef T
#endif
