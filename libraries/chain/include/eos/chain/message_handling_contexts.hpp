#pragma once

#include <eos/chain/message.hpp>
#include <eos/chain/transaction.hpp>
#include <eos/types/types.hpp>

namespace chainbase { class database; }

namespace eos { namespace chain {

class chain_controller;

class apply_context {
    public:
      apply_context(chain_controller& con,
                    chainbase::database& db, 
                    const chain::Transaction& t,
                    const chain::Message& m, 
                    const types::AccountName& code,
                    TransactionAuthorizationChecker* authChecker)
         :controller(con),db(db),trx(t),msg(m),code(code),mutable_controller(con),mutable_db(db),authChecker(authChecker){}

      int32_t store_i64( Name scope, Name table, Name key, const char* data, uint32_t len);
      int32_t remove_i64( Name scope, Name table, Name key );
      int32_t remove_i128i128( Name scope, Name table, uint128_t primary, uint128_t secondary );
      int32_t store_i128i128( Name scope, Name table, uint128_t primary, uint128_t secondary,
                              const char* data, uint32_t len );

      int32_t load_i64( Name scope, Name code, Name table, Name Key, char* data, uint32_t maxlen );
      
      int32_t front_primary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );
      int32_t back_primary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );
      int32_t front_secondary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );
      int32_t back_secondary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );
      int32_t load_primary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );
      int32_t load_secondary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );
      int32_t lowerbound_primary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );
      int32_t lowerbound_secondary_i128i128( Name scope, Name code, Name table, 
                                       uint128_t* primary, uint128_t* secondary, char* data, uint32_t maxlen );

      void require_authorization(const types::AccountName& account);
      void require_scope(const types::AccountName& account)const;
      bool has_recipient( const types::AccountName& account )const;
      void require_recipient(const types::AccountName& account);

      const chain_controller&      controller;
      const chainbase::database&   db;  ///< database where state is stored
      const chain::Transaction&    trx; ///< used to gather the valid read/write scopes
      const chain::Message&        msg; ///< message being applied
      types::AccountName           code; ///< the code that is currently running

      chain_controller&    mutable_controller;
      chainbase::database& mutable_db;

      TransactionAuthorizationChecker* authChecker;

      std::deque<AccountName>          notified;
      std::deque<ProcessedTransaction> sync_transactions; ///< sync calls made 
      std::deque<GeneratedTransaction> async_transactions; ///< async calls requested

};

using apply_handler = std::function<void(apply_context&)>;

} } // namespace eos::chain
