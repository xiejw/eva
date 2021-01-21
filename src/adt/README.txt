vec
        vec is a (very) simple strong-type container. Basically,

                It is a pointer to a auto-grow array. So user can use vec[i] to
                access the elements efficiently. It just feels like a pointer.
                The type information is just correct.

                If the space is not enough, it will self-grow. So, passing
                pointer to the vec (type vec_t(<basic_type>)*) around if it
                needs to change.

                The size and cap are recoreded in the space ahead of the first
                element but invisible to users.

        For simplicity, ownership is on user' side. _vecFree_ just frees the
        resources for the buffer it allocates. All elements are freed by users.

                For example, if users put two heap-allocated objects in vec[0]
                and vec[1], followed by vecSetSzie(vec, 1), this is a potential
                memeory leak as the second item needs some care.

map
        Similarly, map is also a (very) simple strong-type container.

                It auto grows and supports iterations. rvalue is safe and the
                type follows C converstion rule. Ownership is on user's side.
