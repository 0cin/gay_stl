#ifndef _GAYTYPETRAITS_H_
#define _GAYTYPETRAITS_H_

namespace uvwxyz
{
    namespace gay_stl
    {
        struct __gay_true_type   {   };
        struct __gay_false_type   {   };

        template<class Type>
        struct __gay__type_traits
        {
            typedef __gay_false_type has_trivial_default_constructor;
            typedef __gay_false_type has_trivial_copy_constructor;
            typedef __gay_false_type has_trivial_assign;
            typedef __gay_false_type has_trivial_destructor;
            typedef __gay_false_type is_pod;
        };

        template<>
        struct __gay__type_traits<bool>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<char>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<unsigned char>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<signed char>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<wchar_t>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<short>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<unsigned short>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assignment_oeprator;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<int>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<unsigned int>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<long>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<unsigned long>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<long long>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<unsigned long long>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<float>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<double>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<long double>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<char*>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<unsigned char*>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<signed char*>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<const char*>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<const unsigned char*>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };

        template<>
        struct __gay__type_traits<const signed char*>
        {
            typedef __gay_true_type has_trivial_default_constructor;
            typedef __gay_true_type has_trivial_copy_constructor;
            typedef __gay_true_type has_trivial_assign;
            typedef __gay_true_type has_trivial_destructor;
            typedef __gay_true_type is_pod;
        };
    }
}

#endif // _GAYTYPETRAITS_H_
