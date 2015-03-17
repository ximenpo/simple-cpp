#ifndef CREATOR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define CREATOR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

template<typename Intf, typename Type = Intf>
struct creator_intf {
    static Intf*	create();
    static void		destroy(Intf* p);
};

template<typename Intf, typename Type = Intf>
struct creator_null {
    static Intf*	create() {
        return 0;
    }

    static void		destroy(Intf* p) {
    }
};

template<typename Intf, typename Type = Intf>
struct creator_static {
    static Intf*	create() {
        static Type	val;
        return &val;
    }

    static void		destroy(Intf* p) {
    }
};

template<typename Intf, typename Type = Intf>
struct creator_new {
    static Intf*	create() {
        return new Type;
    }

    static void		destroy(Intf* p) {
        delete static_cast<Type*>(p);
    }
};

template<typename Intf, typename Type = Intf>
struct creator_placement_new {
    static Intf*	create() {
        void* p	= std::malloc(sizeof(Type));
        if (!p)	return 0;
        return new(p) Type;
    }

    static void		destroy(Intf* p) {
        static_cast<Type*>(p)->~Type();
        std::free(p);
    }
};

#endif
