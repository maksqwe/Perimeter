
#include "native.h"
#include "hash.h"

# define P0 (PARSE *)0
# define C0 (char *)0


void declare_native_rule(char* module, char* rule, char** args, 
                         LIST*(*f)(PARSE*, FRAME*))

{
    module_t* m = bindmodule(module);
    if (m->native_rules == 0) {
        m->native_rules = hashinit( sizeof( native_rule_t ), "native rules");
    }
    
    {
        native_rule_t n, *np = &n;
        n.name = rule;
        if (args)
        {
            n.arguments = args_new();
            lol_build( n.arguments->data, args );
        }   
        else
        {
            n.arguments = 0;
        }
        n.procedure = parse_make( f, P0, P0, P0, C0, C0, 0 );        
        hashenter(m->native_rules, (HASHDATA**)&np);
    }
}

