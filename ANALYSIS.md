CMSC 124 PS1 Reflection
Marie Toney Fay S. Gelvezon
Seth Leander Caballero

1. Pick 3 of the 10 categories. For each, pick a language that gives it to you for free and say what that language pays for it. "Python has dictionaries" isn't an answer. What does Python's dictionary cost in memory or in speed compared to what you built, and where would you notice? 
    - For maps, Python gives us dict (dictionary), so we don't have to build the hash table from scratch. The trade-off is that python’s dictionary is already optimized to manage references, hashing, and getting the needed data which I think would cost a lot of memory. In comparison to our C implementation, the map we built would have more efficient memory allocation. 

    - Our dt_map uses a SLL (singly linked list) for each hash bucket. The entries are connected through the next pointers and searched one by one when multiple keys produce the same bucket. Meanwhile, Java has a ready-made Linkedlist so we don't have to manually create nodes and maintain links, which also is a downside bcs it uses more memory for its additional object overhead. The trade off is that the elements of java’s linked list come with additional object and reference overhead. In our c version, we have more control over the memory since the node contains only the key, value, and pointer needed for the chain. However, if the number of entries grows for the map, java’s implementation would be more convenient, while ours would require more work manually. 

    - 


2. You wrote the tag check in dt_value_as_int by hand. Some languages don't let you. They make the tagged union a language construct, so the compiler writes the check for you, refuses to compile a read that skips it, and refuses to compile a set of cases that misses one. Rust's enum and match work this way, and so do ML's datatypes and Swift's enumerations with associated values. What does the C version let you do that a compiler enforcing the check wouldn't, and is any of it worth wanting? 
    - In C, the tag check is more of a convention rather than a rule. We write the tag check ourselves, so nothing forces us to do it. We can read as_int on a value that is holding a string or forget a case in a switch and the compiler wouldn't complain. A language like Rust makes examples like the ones we've mentioned compile errors. Basically, C lets us make mistakes without the compiler stopping us. C also lets us control how the data is laid out in memory and treat the same bytes as a different type. This is useful in some cases just like matching a file format, but most of the time it’s not worth wanting. Skipping the tag check is basically just a bug waiting to happen, so this is why our tag/ tests exist. Neither of them is simply better, but they both do what is required of them and their difference is what makes their functionality work for each. In C, we have to remember the check every time, while in Rust the compiler remembers it for us. 

3. Your dt_map keeps insertion order separately from the hash buckets, which is memory spent on something no lookup uses. Argue the other side: describe a design that drops it, say what breaks, and say whether you'd ship it.
    - 

4. Compare access after release with an allocation that remains unreleased at the driver's final check. What damage can each cause in a long-running server? How does that answer change for a command-line tool that exits in a second?
    - 


