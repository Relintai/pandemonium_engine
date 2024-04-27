
def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        #"",
    	"Database",
	    "DatabaseConnection",
	    "DatabaseManager",
	    "DatabaseMultiThreaded",
    	"DatabaseSingleThreaded",
	    "QueryBuilder",
	    "QueryResult",
	    "TableBuilder",
		"PreparedStatement",
    ]

def get_doc_path():
    return "doc_classes"

