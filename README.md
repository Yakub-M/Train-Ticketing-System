# Train-Ticketing-System

This group coursework project involves developing a train ticket processing system through programming. The program features a user interface that displays a menu of options, allowing users to select the type of ticket they wish to purchase and any additional services.

Upon selection, the program processes the user's input in real time, calculating the relevant ticket attributes. Once the user opts to proceed with payment, the program displays a summary of the ticket details along with the total price.

The system utilizes object-oriented principles, with a set of classes designed to handle ticket calculations. The core functionality is encapsulated in an abstract base class, Train, which is inherited by multiple subclasses. Each subclass is responsible for calculating different types of tickets independently. The results are then added to a ticket queue, where tickets can be processed and paid for sequentially.

In a real-world application, this program could be utilized by train ticket handlers using handheld devices similar to POS terminals. These devices would allow ticket handlers to issue tickets on the spot, calculate fares, and process payments efficiently while interacting with passengers.

Additionally, there is a separate class unrelated to the Train hierarchy that manages the ticket queue and tracks the total amount of tickets sold. This data is periodically written to a file, ensuring persistence. Upon exiting the program, the file is properly closed to preserve data integrity.
