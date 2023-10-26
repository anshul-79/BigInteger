#include "BigInteger.h"

#include <stdio.h>
#include <stdlib.h>

// Function to insert a new node at the beginning of a linked list
struct node *insert(struct node *l, int val) {
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  if (!temp) {
    printf("Memory Not Allocated!");
    exit(1);
  }
  temp->data = val;
  temp->next = l;
  return temp;
}
// Function to insert a new node at the end of a BigInteger's linked list
void tail(struct BigInteger *num, int element) {
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  if (temp == NULL) {
    printf("memory allocation failed\n");
    return;
  }
  temp->data = element;
  temp->next = NULL;
  num->length += 1;
  if (num->l == NULL) {
    num->l = temp;
    return;
  }
  struct node *itr = num->l;
  while (itr->next != NULL) {
    itr = itr->next;
  }
  itr->next = temp;
}
// Function to calculate the length of a linked list
long int length(struct node *l) {
  long int count = 0;
  while (l) {
    l = l->next;
    count++;
  }
  return count;
}
// Function to display a linked list in reverse order
void revDisplay(struct node *link) {
  if (!link)
    return;
  revDisplay(link->next);
  printf("%d", link->data);
}
// Function to display a BigInteger
void display(struct BigInteger b) {
  if (b.l == NULL) {
    printf("0");
    return;
  }
  if (b.sign == '-')
    printf("-");
  revDisplay(b.l);
}
// Function to initialize a BigInteger from a string
struct BigInteger initialize(char *s) {
  struct BigInteger b;
  b.l = NULL;
  if (s[0] == '-')
    b.sign = '-';
  else
    b.sign = '+';
  int i;
  if (b.sign == '-')
    i = 1;
  else
    i = 0;
  for (i; s[i] != '\0'; i++) {
    b.l = insert(b.l, s[i] - '0');
  }
  b.length = length(b.l);
  return b;
}
// Function to reverse a linked list
struct node *reverse(struct node *l) {
  struct node *prev = NULL, *front = NULL, *current = l;
  while (current != NULL) {
    front = current->next;
    current->next = prev;
    prev = current;
    current = front;
  }
  return prev;
}
// Function to Compare two BigIntegers
int Compare(struct BigInteger a, struct BigInteger b) {
  if (b.length > a.length) {
    return -1;
  } else if (b.length == a.length) {
    int return_val = 0;
    a.l = reverse(a.l);
    b.l = reverse(b.l);
    struct node *l1 = a.l, *l2 = b.l;
    while (l1 != NULL && l2 != NULL) {
      if (l2->data > l1->data) {
        return_val = -1;
        break;
      } else if (l1->data > l2->data) {
        return_val = 1;
        break;
      }
      l1 = l1->next;
      l2 = l2->next;
    }
    a.l = reverse(a.l);
    b.l = reverse(b.l);
    return return_val;
  }
  return 1;
}
// Function to add two BigIntegers
struct BigInteger add(struct BigInteger a, struct BigInteger b) {
  struct BigInteger result;
  result.l = NULL;
  result.sign = '+';
  result.length = 0;

  if (a.sign == '-' && b.sign == '-') {
    result.sign = '-';
  } else if (a.sign == '-') {
    a.sign = '+';
    result = sub(b, a);
    a.sign = '-';
    return result;
  } else if (b.sign == '-') {
    b.sign = '+';
    result = sub(a, b);
    b.sign = '-';
    return result;
  }

  struct node *l1 = a.l, *l2 = b.l;
  int sum, carry = 0;

  while (l1 != NULL || l2 != NULL || carry != 0) {
    int d1 = (l1 != NULL) ? l1->data : 0;
    int d2 = (l2 != NULL) ? l2->data : 0;

    sum = d1 + d2 + carry;
    carry = sum / 10;
    tail(&result, sum % 10);

    if (l1 != NULL)
      l1 = l1->next;
    if (l2 != NULL)
      l2 = l2->next;
  }
  result.length = length(result.l);
  return result;
}
// Function to subtract two BigIntegers
struct BigInteger sub(struct BigInteger a, struct BigInteger b) {
  struct BigInteger result;
  result.l = NULL;
  result.sign = '+';
  result.length = 0;

  if (a.sign == '-' && b.sign == '+') {
    a.sign = '+';
    result = add(a, b);
    a.sign = '-';
    result.sign = '-';
    return result;
  } else if (a.sign == '+' && b.sign == '-') {
    b.sign = '+';
    result = add(a, b);
    b.sign = '-';
    return result;
  } else if (a.sign == '-' && b.sign == '-') {
    b.sign = '+';
    a.sign = '+';
    result = sub(b, a);
    b.sign = '-';
    a.sign = '-';
    return result;
  } else if (Compare(a, b) == -1) {
    struct BigInteger temp = a;
    a = b;
    b = temp;
  }

  struct node *l1 = a.l, *l2 = b.l;
  int diff, borrow = 0;

  while (l1 != NULL || l2 != NULL) {
    int d1 = (l1 != NULL) ? l1->data : 0;
    int d2 = (l2 != NULL) ? l2->data : 0;

    diff = d1 - d2 - borrow;

    if (diff < 0) {
      diff += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }

    tail(&result, diff);

    if (l1 != NULL)
      l1 = l1->next;
    if (l2 != NULL)
      l2 = l2->next;
  }
  result.l = reverse(result.l);
  while (result.l != NULL && result.l->data == 0) {
    struct node *temp = result.l;
    result.l = result.l->next;
    free(temp);
  }
  result.l = reverse(result.l);
  result.length = length(result.l);
  return result;
}
// Function to multiply two BigIntegers
struct BigInteger mul(struct BigInteger a, struct BigInteger b) {
  struct node *n1 = a.l, *n2 = b.l;
  struct BigInteger result = initialize("");
  result.length = 0;
  if (n1 == NULL || n2 == NULL) {
    return result;
  }
  struct BigInteger mid, res = initialize("");
  int carry = 0, product = 0, i = 0;
  while (n2 != NULL) {
    n1 = a.l;
    carry = 0;
    mid = initialize("");
    for (int j = 0; j < i; j++) {
      tail(&mid, 0);
    }
    while (n1 != NULL) {
      product = (n1->data) * (n2->data) + carry;
      tail(&mid, product % 10);
      carry = product / 10;
      n1 = n1->next;
    }

    if (carry > 0) {
      tail(&mid, carry);
    }

    res = add(mid, res);
    struct node *current = mid.l;
    while (current != NULL) {
      struct node *temp = current;
      current = current->next;
      free(temp);
    }

    n2 = n2->next;
    i++;
  }
  if (a.sign != b.sign) {
    res.sign = '-';
  }
  return res;
}
// Function to divide two BigIntegers
struct BigInteger div1(struct BigInteger a, struct BigInteger b) {
  struct BigInteger result = initialize("0");
  result.length = 0;
  result.sign = '+';
  char sig = '+';
  int flag = 0;
  if (b.length == 1 && b.l->data == 0) {
    printf("Error division by zero");
    return result;
  }
  if (a.sign != b.sign)
    sig = '-';
  char t1 = a.sign;
  char t2 = b.sign;
  a.sign = '+';
  b.sign = '+';
  struct BigInteger temp = add(b, initialize("0"));
  while (Compare(a, temp) >= 0) {
    temp = add(temp, b);
    result = add(result, initialize("1"));
    flag = 1;
  }
  struct BigInteger zero = initialize("0");
  if (flag)
    result.sign = sig;
  a.sign = t1;
  b.sign = t2;
  return result;
}
// Function to find the modulus of two BigIntegers
struct BigInteger mod(struct BigInteger a, struct BigInteger b) {
  struct BigInteger result = initialize("0");
  result.length = 0;
  result.sign = '+';
  char sig = '+';
  int flag = 0;
  if (b.length == 1 && b.l->data == 0) {
    printf("Error division by zero");
    return result;
  }
  if (a.sign != b.sign)
    sig = '-';
  char t1 = a.sign;
  char t2 = b.sign;
  a.sign = '+';
  b.sign = '+';
  struct BigInteger temp = add(b, initialize("0"));
  while (Compare(a, temp) >= 0) {
    temp = add(temp, b);
    result = add(result, initialize("1"));
  }
  struct BigInteger temp2 = sub(temp, b);
  struct BigInteger modu = sub(a, temp2);
  a.sign = t1;
  b.sign = t2;
  return modu;
}