#ifndef TYPES_H
#define TYPES_H
namespace osp2023 {
// renaming some types so that the types are more meaningful
using time_type = long long;
using id_type = int;
// constant that can be used to initialise a time to not having been set
// CHANGE: extern to static
static constexpr time_type time_not_set = -1;
static constexpr time_type time_zero = 0;
}
#endif
