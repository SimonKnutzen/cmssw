#ifndef FWCore_Framework_EventSetupRecord_h
#define FWCore_Framework_EventSetupRecord_h
// -*- C++ -*-
//
// Package:     Framework
// Class  :     EventSetupRecord
//
/**\class EventSetupRecord EventSetupRecord.h FWCore/Framework/interface/EventSetupRecord.h

 Description: Base class for all Records in an EventSetup.  Holds data with the same lifetime.

 Usage:
This class contains the Proxies that make up a given Record.  It
is designed to be reused time after time, rather than it being
destroyed and a new one created every time a new Record is
required.  Proxies can only be added by the EventSetupRecordProvider class which
uses the 'add' function to do this.  The reason for this is
that the EventSetupRecordProvider/DataProxyProvider pair are responsible for
"invalidating" Proxies in a Record.  When a Record
becomes "invalid" the EventSetupRecordProvider must invalidate
all the  Proxies which it does using the DataProxyProvider.

When the set of  Proxies for a Records changes, i.e. a
DataProxyProvider is added of removed from the system, then the
Proxies in a Record need to be changes as appropriate.
In this design it was decided the easiest way to achieve this was
to erase all  Proxies in a Record.

It is important for the management of the Records that each Record
know the ValidityInterval that represents the time over which its data is valid.
The ValidityInterval is set by its EventSetupRecordProvider using the
'set' function.  This quantity can be recovered
through the 'validityInterval' method.

For a Proxy to be able to derive its contents from the EventSetup, it
must be able to access any Proxy (and thus any Record) in the
EventSetup.  The 'make' function of a Proxy provides its
containing Record as one of its arguments.  To be able to
access the rest of the EventSetup, it is necessary for a Record to be
able to access its containing EventSetup.  This task is handled by the
'eventSetup' function.  The EventSetup is responsible for managing this
using the 'setEventSetup' and 'clearEventSetup' functions.

*/
//
// Author:      Chris Jones
// Created:     Fri Mar 25 14:38:35 EST 2005
//


// user include files
#include "FWCore/Framework/interface/DataKey.h"
#include "FWCore/Framework/interface/NoProxyException.h"
#include "FWCore/Framework/interface/ValidityInterval.h"
#include "FWCore/Utilities/interface/ESInputTag.h"

// system include files
#include <map>
#include <vector>

// forward declarations
namespace cms {
   class Exception;
}

namespace edm {
   class ESInputTag;
   class EventSetup;

   namespace eventsetup {
      struct ComponentDescription;
      class DataProxy;
      class EventSetupRecordKey;

      class EventSetupRecord {

      public:
         EventSetupRecord();
         virtual ~EventSetupRecord();

         // ---------- const member functions ---------------------
         ValidityInterval const& validityInterval() const {
            return validity_;
         }

         template<typename HolderT>
         void get(HolderT& iHolder) const {
            typename HolderT::value_type const* value = 0;
            ComponentDescription const* desc = 0;
            this->getImplementation(value, "", desc, iHolder.transientAccessOnly);

            iHolder = HolderT(value, desc);
         }

         template<typename HolderT>
         void get(char const* iName, HolderT& iHolder) const {
            typename HolderT::value_type const* value = 0;
            ComponentDescription const* desc = 0;
            this->getImplementation(value, iName, desc, iHolder.transientAccessOnly);
            iHolder = HolderT(value, desc);
         }
         template<typename HolderT>
         void get(std::string const& iName, HolderT& iHolder) const {
            typename HolderT::value_type const* value = 0;
            ComponentDescription const* desc = 0;
            this->getImplementation(value, iName.c_str(), desc, iHolder.transientAccessOnly);
            iHolder = HolderT(value, desc);
         }

         template<typename HolderT>
         void get(ESInputTag const& iTag, HolderT& iHolder) const {
            typename HolderT::value_type const* value = 0;
            ComponentDescription const* desc = 0;
            this->getImplementation(value, iTag.data().c_str(), desc, iHolder.transientAccessOnly);
            validate(desc, iTag);
            iHolder = HolderT(value, desc);
         }



         ///returns false if no data available for key
         bool doGet(DataKey const& aKey, bool aGetTransiently = false) const;

         /**returns true only if someone has already requested data for this key
          and the data was retrieved
          */
         bool wasGotten(DataKey const& aKey) const;

         /**returns the ComponentDescription for the module which creates the data or 0
          if no module has been registered for the data. This does not cause the data to
          actually be constructed.
          */
         ComponentDescription const* providerDescription(DataKey const& aKey) const;

         virtual EventSetupRecordKey key() const = 0;

         /**If you are caching data from the Record, you should also keep
          this number.  If this number changes then you know that
          the data you have cached is invalid. This is NOT true if
          if the validityInterval() hasn't changed since it is possible that
          the job has gone to a new Record and then come back to the
          previous SyncValue and your algorithm didn't see the intervening
          Record.
          The value of '0' will never be returned so you can use that to
          denote that you have not yet checked the value.
          */
         unsigned long long cacheIdentifier() const {
            return cacheIdentifier_;
         }

         ///clears the oToFill vector and then fills it with the keys for all registered data keys
         void fillRegisteredDataKeys(std::vector<DataKey>& oToFill) const;
         // ---------- static member functions --------------------

         // ---------- member functions ---------------------------

         // The following member functions should only be used by EventSetupRecordProvider
         bool add(DataKey const& iKey ,
                  DataProxy const* iProxy) ;
         void cacheReset() ;
         /// returns 'true' if a transient request has occurred since the last call to transientReset.
         bool transientReset() ;
         void set(ValidityInterval const&);
         void setEventSetup(EventSetup const* iEventSetup) {eventSetup_ = iEventSetup; }
      protected:

         DataProxy const* find(DataKey const& aKey) const ;

         EventSetup const& eventSetup() const {
            return *eventSetup_;
         }

         void validate(ComponentDescription const*, ESInputTag const&) const;

         void addTraceInfoToCmsException(cms::Exception& iException, char const* iName, ComponentDescription const*, DataKey const&) const;
         void changeStdExceptionToCmsException(char const* iExceptionWhatMessage, char const* iName, ComponentDescription const*, DataKey const&) const;

         void transientAccessRequested() const { transientAccessRequested_ = true;}
      private:
         EventSetupRecord(EventSetupRecord const&); // stop default

         EventSetupRecord const& operator=(EventSetupRecord const&); // stop default

         void const* getFromProxy(DataKey const& iKey ,
                                  ComponentDescription const*& iDesc,
                                  bool iTransientAccessOnly) const;

         template <typename DataT>
         void getImplementation(DataT const*& iData ,
                                char const* iName,
                                ComponentDescription const*& iDesc,
                                bool iTransientAccessOnly) const {
            DataKey dataKey(DataKey::makeTypeTag<DataT>(),
                            iName,
                            DataKey::kDoNotCopyMemory);

            void const* pValue = this->getFromProxy(dataKey, iDesc, iTransientAccessOnly);
            if(0 == pValue) {
               throw NoProxyException<DataT>(this->key(), dataKey);
            }
            iData = reinterpret_cast<DataT const*> (pValue);
         }

         // ---------- member data --------------------------------
         ValidityInterval validity_;
         std::map<DataKey, DataProxy const*> proxies_ ;
         EventSetup const* eventSetup_;
         unsigned long long cacheIdentifier_;
         mutable bool transientAccessRequested_;
      };
   }
}
#endif
