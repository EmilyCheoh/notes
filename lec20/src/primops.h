#pragma once

#include "value.h"
#include "env.h"

namespace islpp {

namespace primop {

// The initial environment.
extern env_ptr<value_ptr> environment;

//
// A few primop values, for testing.
//

extern value_ptr cons;
extern value_ptr plus;
extern value_ptr num_eq;
extern value_ptr equal_huh;

}

}
