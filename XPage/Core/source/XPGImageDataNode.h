

#ifndef __XPGImageDataNode_H_DEFINED__
#define __XPGImageDataNode_H_DEFINED__

/** Class to represent a generic node representing image data within a tree
*/

class XPGImageDataNode
{
public:

	enum NodeType { kRubriqueNode = 0, kImageNode } ;

	typedef struct imagedata 
	{
		PMString name, articleID, legend, credit;
		PMString type, brFile, hrFileNB;
	} ImageData;

	/** Constructor */
	XPGImageDataNode(NodeType type);

	/** Destructor */
	virtual ~XPGImageDataNode();

	// +K2Vector support

	/**	Copy constructor
		@param rhs [IN] node reference
		@return  
	 */
	XPGImageDataNode(const XPGImageDataNode& rhs);

	/**	Operator assignment
		@param rhs [IN] node reference
		@return XPGImageDataNode& 
	 */
	XPGImageDataNode& operator=(const XPGImageDataNode& rhs);

	/** For K2Vector copy semantics
	*/
	typedef object_type data_type;
	// -K2Vector support

	/**	Operator equality
		@param rhs [IN] node reference
		@return bool 
	 */
	bool operator==(const XPGImageDataNode& rhs) const;


	/**	Accessor for child by zerobased index in parent's list of kids
		@param indexInParent [IN] which child by zero-based index
		@return child at given index 
	 */
	const XPGImageDataNode& GetNthChild(int32 indexInParent) const;

	/**	Accessor for parent
		@return reference to parent node
	 */
	XPGImageDataNode* GetParent() const;

	/**	Accessor for size of child list on this node
		@return int32 giving the number of children on this node 
	 */
	int32 ChildCount() const;

	/**	Add child to this node (at end of list)
		@param o [IN] child node to add
	 */
	void AddChild(const XPGImageDataNode& o);

	/**	Remove specified child from our list
		@param o [IN] specifies child to remove
	 */
	void RemoveChild(const XPGImageDataNode& o);

	/** Mutator for the parent of this node
		@param p [IN] specifies new parent to set
	 */
	void SetParent(const XPGImageDataNode* p);

	/**
		Data accessors for image node 
	*/

	PMString GetID() const; // ID is the file path
	void SetID(const PMString& id);
	
	NodeType GetType() const;
	
	PMString GetData() const; // data is the image absolute file path for kImage nodes, it's a rubrique name for kRubrique nodes
	void SetData(const PMString& data);
	
	void SetImageData(const PMString& name, const PMString& articleID, const PMString& legend, const PMString& credit,
					  const PMString& type = kNullString, const PMString& brFile = kNullString, const PMString& hrFileNB = kNullString);
	const ImageData * GetImageData() const; // Image data is data extracted from description file for kImageNode nodes, it's nil for kRubrique nodes

private:

	K2Vector<XPGImageDataNode> fChildren;	
	PMString fIDData, fData;
	NodeType type;
	ImageData * imageData;
	XPGImageDataNode* fParent;
	void deepcopy(const XPGImageDataNode& rhs);
};

#endif // __XPGImageDataNode_H_DEFINED__

