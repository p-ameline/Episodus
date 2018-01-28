//
//  binder_macros.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef BINDER_MACROS_H_KRISTIANLIPPERT_20041118_1631
#define BINDER_MACROS_H_KRISTIANLIPPERT_20041118_1631

namespace dti{
namespace db{

#define BEGIN_DB_BINDING template< class RECORD, class OPERATION> \
static void create_bindings(OPERATION & sb)\
{\
RECORD layout_holder;

#define DB_BIND_COLUMN_NAME(VAR, COLUMNNAME)                 sb.bind(layout_holder, dti::db::from_db, layout_holder.VAR, COLUMNNAME );
#define DB_BIND_COLUMN_IDX(VAR, COLUMNIDX)                   sb.bind(layout_holder, dti::db::from_db, layout_holder.VAR, COLUMNIDX );
#define DB_BIND_COLUMN_AUTOIDX(VAR)                          sb.bind(layout_holder, dti::db::from_db, layout_holder.VAR);
#define DB_BIND_ATT_COLUMN_NAME(ATTRIBUTES, VAR, COLUMNNAME) sb.bind(layout_holder, ATTRIBUTES, layout_holder.VAR, COLUMNNAME );
#define DB_BIND_ATT_COLUMN_IDX(ATTRIBUTES, VAR, COLUMNIDX)   sb.bind(layout_holder, ATTRIBUTES, layout_holder.VAR, COLUMNIDX );
#define DB_BIND_ATT_COLUMN_AUTOIDX(ATTRIBUTES, VAR)          sb.bind(layout_holder, ATTRIBUTES, layout_holder.VAR);

#define DB_BIND_COLUMN_NAME_EXTRA(VAR, COLUMNNAME, EXTRA)                 sb.bind(layout_holder, dti::db::from_db, layout_holder.VAR, COLUMNNAME );
#define DB_BIND_COLUMN_IDX_EXTRA(VAR, COLUMNIDX, EXTRA)                   sb.bind(layout_holder, dti::db::from_db, layout_holder.VAR, COLUMNIDX, EXTRA );
#define DB_BIND_COLUMN_AUTOIDX_EXTRA(VAR, EXTRA)                          DB_BIND_COLUMN_NAME_EXTRA(VAR, "", EXTRA)
#define DB_BIND_ATT_COLUMN_NAME_EXTRA(ATTRIBUTES, VAR, COLUMNNAME, EXTRA) sb.bind(layout_holder, ATTRIBUTES, layout_holder.VAR, COLUMNNAME );
#define DB_BIND_ATT_COLUMN_IDX_EXTRA(ATTRIBUTES, VAR, COLUMNIDX, EXTRA)   sb.bind(layout_holder, ATTRIBUTES, layout_holder.VAR, COLUMNIDX, EXTRA );
#define DB_BIND_ATT_COLUMN_AUTOIDX_EXTRA(ATTRIBUTES, VAR, EXTRA)          DB_BIND_ATT_COLUMN_NAME_EXTRA(ATTRIBUTES, VAR, "", EXTRA)

#define END_DB_BINDING } 

} // namespace db
} // namespace dti

#endif // BINDER_MACROS_H_KRISTIANLIPPERT_20041118_1631
