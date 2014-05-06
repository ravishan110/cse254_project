
///DESIGN AND ANALYSIS OF ALGORITHMS PROJECT
///       Ravi Shankar Bharti     1200128
///           Vikram Patel        1200123
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>


typedef struct node_t {
	struct node_t *left, *right;
	int freq;
	char c;
} *node;

struct node_t pool[256] = {{0}};
node qqq[255], *q = qqq - 1;
int n_nodes = 0, qend = 1,tot_len;
char *code[128] = {0}, buf[1024];

int prime(int n)///gives prime number grater than 'n'
{
    int flag,i,j;
    for(i=n+1;;i++)
    {
        flag=0;
        for(j=2;j<=sqrt(i);j++)
        {
            if(i%j == 0)
            {
                flag=1;
            }
        }
        if(flag == 0)
        {
            return i;
        }
    }
}
///finds modular inverse of r (mod q).....
int modular_inverse(int r , int q)
{
     int i = q, v = 0, d = 1,s=r;

      while (s > 0)
      {
        int t = i/s, x = s;
        s = i % x;
        i = x;
        x = d;
        d = v - t * x;
        v = x;
      }

      v %= q;
      if (v < 0) v = (v+q) % q;
    return v;
}

node new_node(int freq, char c, node a, node b)
{
	node n = pool + n_nodes++;
	if (freq) n->c = c, n->freq = freq;
	else {
		n->left = a, n->right = b;
		n->freq = a->freq + b->freq;
	}
	return n;
}

/* priority queue */
void qinsert(node n)
{
	int j, i = qend++;
	while ((j = i / 2)) {
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}

node qremove()
{
	int i, l;
	node n = q[i = 1];

	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}

/// walk the tree and put 0s and 1s
void build_code(node n, char *s, int len)
{
	static char *out = buf;
	if (n->c) {
		s[len] = 0;
		strcpy(out, s);
		code[n->c] = out;
		out += len + 1;
		return;
	}

	s[len] = '0'; build_code(n->left,  s, len + 1);
	s[len] = '1'; build_code(n->right, s, len + 1);
}



///Generates knapsack of length n...
int * generate_knapsack(int *s,int n)
{
    int x,i,j;
    s[0]=0;
    for(i=0;i<n;i++)
    {
        x=0;
        for(j=0;j<i;j++)
        {
            x=x+s[j];
        }
        s[i] =x+1;
    }
}


void init(const char *s)
{
	int i, freq[128] = {0};
	char c[16];

	while (*s)
    {
        freq[(int)*s++]++;
    }

	for (i = 0; i < 128; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));

	while (qend > 2)
		qinsert(new_node(0, 0, qremove(), qremove()));

	build_code(q[1], c, 0);
}

void encode(const char *s, char *out)
{
	while (*s) {
		strcpy(out, code[*s]);
		out += strlen(code[*s++]);
	}
}

void decode(const char *s, node t)
{
	node n = t;
	int i;
    for(i=0;i<=tot_len;i++)
    {
		if (*s++ == '0') n = n->left;
		else n = n->right;

		if (n->c) putchar(n->c) ,n = t;
	}

	putchar('\n');
}

int main()
{
    srand(time(0));
    int n,i,c=0,j,k,x,extra_bits;
    n=8;
    char str[1024];
    printf("\nEnter your Message : ");
    gets(str);

    ///public knapsack and private knapsack...
    int priv_knap[n],pub_knap[n],p,r;

    printf("\nCreating Huffman tree and generating the code...\n");
    init(str);
    printf("\n");
    printf("\ngenerated code for different characters :\n");
	for (i = 0; i < 128; i++)
    {
        if (code[i]) printf("'%c': %s\n", i, code[i]);
    }

    char buf[1024];
    encode(str, buf);
    printf("\nHuffman Encoded String : %s",buf);

    while(buf[c] == '0' || buf[c] == '1')
    {
            c++;
    }

    char enc_huf_str[c];
    for(i=0;i<c;i++)
    {
        enc_huf_str[i]=buf[i];
    }

    tot_len=sizeof(enc_huf_str)/sizeof(enc_huf_str[0]);
    extra_bits=0;
    ///calculating extra bits that is needed to be added...
    if(tot_len%n !=0)
    {
        extra_bits = n - tot_len% n ;
    }

    char coded_str[tot_len+extra_bits];
    for(i=0;i<extra_bits+tot_len;i++)
    {
            if(i<tot_len)coded_str[i]=enc_huf_str[i];
            else coded_str[i]='0';
    }

    int cipher_text[strlen(coded_str)/n],decipher_text[strlen(coded_str)/n];
    tot_len=sizeof(coded_str)/sizeof(coded_str[0]);

    printf("\n\nGenerating private Knapsack ...\n");
    generate_knapsack(priv_knap,n);///generating private knapsack...

    x=0;
    /// "x" will denote the sum of all the elements of knapsack...
    for(i=0;i<n;i++)
    {
        x=x+priv_knap[i];
    }
    printf("\nPrivate knapsack : \n");
    for(i=0;i<n;i++)
    {
        printf("%d ",priv_knap[i]);
    }
    p = prime(x);/// prime no. greater than sum off all the elements of the knapsack....
    printf("\n");
    r = p - rand()%(p/7);/// number greater than '0' but more than 'p'
    printf("\nr:%d \np:%d\n",r,p);

    printf("\nGenerating Public Knapsack...\n\n");

    printf("Public Knapsack : \n");
    for(i=0;i<n;i++)
    {
        pub_knap[i] =(priv_knap[i]*r)%p;
        printf("%d ",pub_knap[i]);
    }
    printf("\n\ncreating cipher text...\n");
    k=0;
    for(i=0;i<tot_len;i=i+n)
    {
        cipher_text[i/n]=0;
        for(j=0;j<n;j++)
        {
            cipher_text[i/n] += pub_knap[j]*(coded_str[k]-'0');
            k++;
        }
    }
    printf("\ncipher text : \n");
    for(i=0;i<tot_len/n;i++)
    {
        printf("%d ",cipher_text[i]);
    }

    ///modular inverse of q(mod p)
    int mod_inv = modular_inverse(r,p);
    printf("\n\nmodular inverse of %d (mod %d): %d\n",r,p,mod_inv);

    printf("\nDeciphering the Cipher text using private key\n");
    for(i=0;i<tot_len/n;i++)
    {
        decipher_text[i]= (cipher_text[i]*mod_inv)%p;
    }

    printf("\nDecipher text : \n");
    for(i=0;i<tot_len/n;i++)
    {
       printf("%d  ", decipher_text[i]);
    }
    char temp_text[tot_len],text[tot_len-extra_bits];
    int l;
    printf("\n\nCreating original string...\n");
    for(i=0;i<tot_len/n;i++)
    {
        for(j=n-1;j>=0;j--)
        {
            if(decipher_text[i] >= priv_knap[j])
            {
                decipher_text[i] -= priv_knap[j];
                temp_text[n*i+j]='1';
            }
            else{
                temp_text[n*i+j]='0';
            }
        }
    }
    tot_len = tot_len - extra_bits;
    for(i=0;i<tot_len;i++)
    {
        text[i]=temp_text[i];
    }
    printf("\noriginal text : \n");
    for(i=0;i<tot_len;i++)
    {
        printf("%c",text[i]);
    }
    printf("\n\ndecoded Message: ");
	decode(text, q[1]);
    return 0;
}

