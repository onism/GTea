#ifndef REMOVEDATA_H
#define REMOVEDATA_H
#include <osg/NodeVisitor>

#include <vector>
#include <string>
#include <osg/Geometry>
#include <osg/NodeVisitor>
class RemoveData : public osg::NodeVisitor
{
public:
    RemoveData( unsigned int flags=DEFAULT, const osg::NodeVisitor::TraversalMode travMode=osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
    ~RemoveData();

    enum RemovalFlags {
            STATESETS = (0x1 << 0),
            STATESET_TEXTURES = (0x1 << 1),
            EMPTY_STATESETS = (0x1 << 2),
            DRAWABLES = (0x1 << 3),
            GEOMETRY_ARRAYS = (0x1 << 4),
            GEOMETRY_PRIMITIVESETS = (0x1 << 5),
            GEODES = (0x1 << 6),
            USERDATA = (0x1 << 7),
            DESCRIPTIONS = (0x1 << 8),
            DEFAULT = ( STATESETS |
                DRAWABLES |
                DESCRIPTIONS ),
            ALL = ( STATESETS | STATESET_TEXTURES |
                DRAWABLES | GEOMETRY_ARRAYS | GEOMETRY_PRIMITIVESETS |
                GEODES | USERDATA |
                DESCRIPTIONS )
        };
        void setRemovalFlags( unsigned int flags );
        unsigned int getRemovalFlags() const;

        void addRemoveMode( GLenum mode );
        void addRemoveAttribute( osg::StateAttribute::Type attribute );

        static std::string flagsToString( unsigned int flags );
        static unsigned int stringToFlags( const std::string& str );

        virtual void apply( osg::Node& node );
        virtual void apply( osg::Group& node );
        virtual void apply( osg::Geode& node );

    protected:
        unsigned int _removalFlags;

        void apply( osg::StateSet* ss );
        void apply( osg::Geometry& geom );
        bool isEmpty( const osg::StateSet& stateSet );

        typedef std::vector< GLenum > ModeVector;
        typedef std::vector< osg::StateAttribute::Type > AttributeTypeVector;
        ModeVector _removeModes;
        AttributeTypeVector _removeAttrs;
};

#endif // REMOVEDATA_H
