
typedef struct Set {
  element_t rep;
} Set

Set* SetMake(element_t node);
Set* SetUnion(Set* A, Set* B);
Set* Setintersect(Set* A, Set* B);
void SetDelete(Set* S);