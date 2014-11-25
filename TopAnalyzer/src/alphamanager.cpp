#include "alphamanager.h"
#include "logger.h"

#define TIXML_USE_STL

#include <tinyxml.h>

using namespace knl;

const unsigned int NUM_INDENTS_PER_SPACE=2;

const char * getIndent( unsigned int numIndents )
{
  static const char * pINDENT="                                      + ";
  static const unsigned int LENGTH=strlen( pINDENT );
  unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
  if ( n > LENGTH ) n = LENGTH;

  return &pINDENT[ LENGTH-n ];
}

// same as getIndent but no "+" at the end
const char * getIndentAlt( unsigned int numIndents )
{
  static const char * pINDENT="                                        ";
  static const unsigned int LENGTH=strlen( pINDENT );
  unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
  if ( n > LENGTH ) n = LENGTH;

  return &pINDENT[ LENGTH-n ];
}

int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
  if ( !pElement ) return 0;

  TiXmlAttribute* pAttrib=pElement->FirstAttribute();
  int i=0;
  int ival;
  double dval;
  const char* pIndent=getIndent(indent);
  printf("\n");
  while (pAttrib)
  {
    printf( "%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());

    if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
    if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
    printf( "\n" );
    i++;
    pAttrib=pAttrib->Next();
  }
  return i;
}

void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 )
{
  if ( !pParent ) return;

  TiXmlNode* pChild;
  TiXmlText* pText;
  int t = pParent->Type();
  printf( "%s", getIndent(indent));
  int num;

  switch ( t )
  {
  case TiXmlNode::TINYXML_DOCUMENT:
    printf( "Document" );
    break;

  case TiXmlNode::TINYXML_ELEMENT:
    printf( "Element [%s]", pParent->Value() );
    num=dump_attribs_to_stdout(pParent->ToElement(), indent+1);
    switch(num)
    {
      case 0:  printf( " (No attributes)"); break;
      case 1:  printf( "%s1 attribute", getIndentAlt(indent)); break;
      default: printf( "%s%d attributes", getIndentAlt(indent), num); break;
    }
    break;

  case TiXmlNode::TINYXML_COMMENT:
    printf( "Comment: [%s]", pParent->Value());
    break;

  case TiXmlNode::TINYXML_UNKNOWN:
    printf( "Unknown" );
    break;

  case TiXmlNode::TINYXML_TEXT:
    pText = pParent->ToText();
    printf( "Text: [%s]", pText->Value() );
    break;

  case TiXmlNode::TINYXML_DECLARATION:
    printf( "Declaration" );
    break;
  default:
    break;
  }
  printf( "\n" );
  for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
  {
    dump_to_stdout( pChild, indent+1 );
  }
}

// load the named file and dump its structure to STDOUT
void dump_to_stdout(const char* pFilename)
{
  TiXmlDocument doc(pFilename);
  bool loadOkay = doc.LoadFile();
  if (loadOkay)
  {
    printf("\n%s:\n", pFilename);
    dump_to_stdout( &doc ); // defined later in the tutorial
  }
  else
  {
    printf("Failed to load file \"%s\"\n", pFilename);
  }
}


bool AlphaManager::init(std::string path)
{
  m_alphaPath = path;

  TiXmlDocument metafile(path + std::string("metafile.xml"));
  bool load_ok = metafile.LoadFile();

  if(!load_ok) return false;

  dump_to_stdout((path + std::string("metafile.xml")).c_str());

  TiXmlNode* node = NULL;
  TiXmlElement* data_elem = NULL;

  node = metafile.FirstChild("alpha-maps");
  if(node == NULL) return true;

  data_elem = node->ToElement();

  for(TiXmlNode* data_child = node->FirstChild(); data_child != NULL; data_child = data_child->NextSibling()) {
    Dataset* d = new Dataset;

    std::istringstream(data_child->FirstChild("width")->FirstChild()->Value()) >> d->width;
    std::istringstream(data_child->FirstChild("height")->FirstChild()->Value()) >> d->height;
    std::istringstream(data_child->FirstChild("slices")->FirstChild()->Value()) >> d->slices;
    std::istringstream(data_child->FirstChild("bytes_elem")->FirstChild()->Value()) >> d->bytes_elem;

    std::string alpha_key = data_child->FirstChild("name")->FirstChild()->Value();
    std::string data_key = data_child->FirstChild("dataset")->FirstChild()->Value();
    add(alpha_key, data_key, d);
  }

  return true;
}

bool AlphaManager::add(std::string key, std::string data_key, Dataset* alpha_map)
{
  if(key.empty() || data_key.empty() || alpha_map == NULL) {
    Logger::getInstance()->error("AlphaManager::Add - invalid parameters. Returning now.");
    return false;
  }

  m_alphaMap[key] = new Dataset(*alpha_map);
  m_alphaDataMap[key] = data_key;
  return true;
}

bool AlphaManager::setActive(std::string key, GLenum tex_unit)
{
  if(key.empty() || m_activeKey == key) return true;

  std::map<std::string, Dataset*>::iterator it = m_alphaMap.find(key);

  if(it == m_alphaMap.end()) return false;

  Dataset* data = it->second;
  if(!data->isLoaded()) {
    if(!data->load(m_alphaPath + it->first + ".raw")) {
      Logger::getInstance()->error("Failed to load alpha map " + key);
      return false;
    }
  }
  if(!data->isUploaded()) {
    if(!data->upload()) {
      Logger::getInstance()->error("Failed to upload alpha map " + key);
      return false;
    }
  }

  data->bind(tex_unit);
  m_activeKey = key;
  return true;
}

bool AlphaManager::saveCurrent()
{
  if(!getCurrent())
    return false;

  TiXmlDocument metafile(m_alphaPath + std::string("metafile.xml"));
  bool load_ok = metafile.LoadFile();
  if(!load_ok) return false;

  TiXmlElement* root = metafile.FirstChildElement("alpha-maps");

  if(root) {
    char buf[20];
    knl::Dataset* alpha_map = getCurrent();

    TiXmlElement* key = new TiXmlElement("name");
    TiXmlText* tkey = new TiXmlText(getCurrentKey());
    key->LinkEndChild(tkey);

    TiXmlElement* data_key = new TiXmlElement("dataset");
    TiXmlText* tdata_key = new TiXmlText(m_alphaDataMap.find(getCurrentKey())->second);
    data_key->LinkEndChild(tdata_key);

    TiXmlElement* width = new TiXmlElement("width");
    sprintf(buf, "%d", alpha_map->width);
    TiXmlText* twidth = new TiXmlText(buf);
    width->LinkEndChild(twidth);

    TiXmlElement* height = new TiXmlElement("height");
    sprintf(buf, "%d", alpha_map->height);
    TiXmlText* theight = new TiXmlText(buf);
    height->LinkEndChild(theight);

    TiXmlElement* slices = new TiXmlElement("slices");
    sprintf(buf, "%d", alpha_map->slices);
    TiXmlText* tslices = new TiXmlText(buf);
    slices->LinkEndChild(tslices);

    TiXmlElement* bytes_elem = new TiXmlElement("bytes_elem");
    sprintf(buf, "%d", alpha_map->bytes_elem);
    TiXmlText* tbytes_elem = new TiXmlText(buf);
    bytes_elem->LinkEndChild(tbytes_elem);

    TiXmlElement* data = new TiXmlElement("data");

    data->LinkEndChild(key);
    data->LinkEndChild(data_key);
    data->LinkEndChild(width);
    data->LinkEndChild(height);
    data->LinkEndChild(slices);
    data->LinkEndChild(bytes_elem);

    root->LinkEndChild(data);
  }

  metafile.SaveFile();
}

Dataset* AlphaManager::get(std::string key)
{
  if(key.empty() || m_alphaMap.find(key) == m_alphaMap.end())
    return NULL;
  return m_alphaMap[key];
}

Dataset* AlphaManager::getCurrent()
{
  return get(m_activeKey);
}

void AlphaManager::freeResources()
{
  for(auto it = m_alphaMap.begin(); it != m_alphaMap.end(); it++) {
    if(it->second->tex_id != 0)
      glDeleteTextures(1, &it->second->tex_id);
    delete it->second;
  }

  m_alphaMap.clear();
}
