/**
 * @file dllist.h
 * @brief Double Linked List for AoC
 */

struct dllist;
typedef struct dllist dllist_t;

struct node;
typedef struct node node_t;

/**
 * @brief Initialize empty Double Linked List
 *
 * @returns An empty dllist
 */
dllist_t* init_list();

/**
 * @brief Frees an entire dllist
 *
 * This function frees all memory allocated by the list.
 *
 * NOTE: The user needs to free the actual data.
 */
void free_dllist(dllist_t* l);

/**
 * @brief Returns a pointer to the data store in @e n
 *
 * @param n Node from which to fetch the data member
 * @returns Pointer to the data held in the node
 */
void* data(const node_t* n);

/**
 * @brief Appends a node containing @e data to the end of the list
 *
 * NOTE: The user is reponsbile for handling the memory associated
 * with @e data.
 *
 * @param l The list to append the new node to
 * @param data The data the node should store
 * @returns A pointer to the newly inserted node
 */
node_t* append(dllist_t* l, void* data);

/**
 * @brief Removes the node @e n
 *
 * Node @e n is removed from the list, connecting it's successor
 * with it's predecessor.
 *
 * The returned node is @e n's successor.
 *
 * @param n The node to remove
 * @returns @e n's former successor
 */
node_t* remove_node(dllist_t* l, node_t* n);

/**
 * @brief Returns the next node in the list from @e n
 *
 * If @e has a successor, a pointer to it is returned. Otherwise,
 * NULL is returned, meaning that @e n was the last node in the list.
 *
 * @param n Return the successor of this node
 * @returns @e n's successor, if any, NULL otherwise
 */
node_t* next(node_t* n);

/**
 * @brief Returns the previous node in the list from @e n
 *
 * If @e has a predecessor, a pointer to it is returned. Otherwise,
 * NULL is returned, meaning that @e n was the first node in the list.
 *
 * @param n Return the predecessor of this node
 * @returns @e n's predecessor, if any, NULL otherwise
 */
node_t* prev(node_t* n);

/**
 * @brief Returns the first node of @e l
 *
 * @param l Return the first node of this list
 * @returns The first node of @e l or NULL if @e l is empty
 */
node_t* head(dllist_t* l);

/**
 * @brief Returns the last node of @e l
 *
 * @param l Return the last node of this list
 * @returns The last node of @e l or NULL if @e l is empty
 */
node_t* tail(dllist_t* l);
