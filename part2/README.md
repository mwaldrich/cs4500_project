For our Queue interfaces, ObjectQueue and StringQueue, we focused on implementing on the most basic / expected methods 
for this data structure - ie pop, push, peek, and size - to keep our inital design simple and easy to test with. We can
easily imagine other less commonly used methods that might be of use to some users of the API such as resizing,
insert at given index, remove from a given index, rotating the deque, reversal, etc. - but again, given the more niche 
usage of these functionalities and generally longer runtimes (more O(n) operations than O(1)) decided to not implement
them in this assignment. If needed in future assignments, their addition would be easy.

For our testing of our interfaces, instead of grouping our tests into small functions that each tested one function,
we decided to group the testing of certain functions based on how likely they were to be used with other methods. That 
led us to three groups - a set of "normal" usages, "advanced" usages, and typical Object equality and hashing usages. 
The set of normal usages consisted of basic operations (like mentioned above) like adding items, removing items, 
checking the size, and peeking at the top of the queue - much like how a normal user of this interface would likely use
it. Our advanced usages consisted of our more niche and less likely to be used functions add_all and clear, for the 
testing of all a whole bunch of items to a Queue at once, and removing a whole bunch of items from a Queue at once. Our
final set of tests with equality & hashing tested typical equality & hashing rules that apply to all Objects, especially
mutable containers (changing elements changes the hash and equality).