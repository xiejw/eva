vec
        vec is a (very) simple container. Basically,

                It is a pointer to a auto-grow array. So user can use vec[i] to
                access the elements efficiently. It just feels like a pointer.

                If the space is not enough, it will self-grow.

                The size and cap are recoreded in the space ahead of the first
                element but invisible to users.

        For simplicity, ownership is on user' side. _vecFree_ just frees the
        resources for the buffer it allocates. All elements are freed by users.

