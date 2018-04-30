# Memory Manager

All values allocated on the heap are associated with a scope ID (all scopes have
an ID), this is updated when a heap object is referenced from another scope. At
the end of a scope, any values associated with it are freed.
