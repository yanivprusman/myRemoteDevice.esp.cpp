find . ! -name 'cleanStart.sh' ! -name '.' -exec rm -rf {} +
echo "
namespace MRD;
enum types:byte {server=0,client=1}
enum actions:byte {getPage=0}
table MRD{
    type:types;
    action:actions;
    data:string;
}
root_type MRD;

">mrd.fbs