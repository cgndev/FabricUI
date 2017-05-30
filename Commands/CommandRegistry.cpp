//
// Copyright (c) 2010-2017 Fabric Software Inc. All rights reserved.
//

#include "CommandRegistry.h"
#include <FabricUI/Util/TypeInfo.h>
#include <FabricUI/Application/FabricException.h>

using namespace FabricUI;
using namespace Util;
using namespace Commands;
using namespace Application;

bool CommandRegistry::s_instanceFlag = false;
CommandRegistry* CommandRegistry::s_cmdRegistry = 0;

CommandRegistry::CommandRegistry() 
  : Util::BaseFactoryRegistry()
{
  if(s_instanceFlag)
    FabricException::Throw(
      "CommandRegistry::CommandRegistry",
      "singleton has already been created"
      );
   
  COMMAND_CPP = "CPP";
 
  // Set the pointer of the CommandRegistry singleton
  // equal to this instance of CommandRegistry.
  s_cmdRegistry = this;
  s_instanceFlag = true;
}

CommandRegistry::~CommandRegistry() 
{
  s_instanceFlag = false;
}

CommandRegistry* CommandRegistry::GetCommandRegistry()
{
  if(!s_instanceFlag)
    FabricException::Throw(
      "CommandRegistry::GetCommandRegistry",
      "the registry is null"
      );

  return s_cmdRegistry;
}

bool CommandRegistry::IsInitalized()
{
  return s_instanceFlag;
}

void CommandRegistry::registerFactory(
  const QString &name, 
  Factory *factory) 
{
  if(!isCommandRegistered(name))
  {
    Util::BaseFactoryRegistry::registerFactory(name, factory);

    commandIsRegistered(
      name,
      // Get the name of the cmd class.
      // --> FactoryClassName<CmdClassName>
      factory->getType(),
      COMMAND_CPP);
  }
}

void CommandRegistry::unregisterFactory(
  const QString &name)
{
  // Does nothing.
}

bool CommandRegistry::isCommandRegistered(
  const QString &cmdName) 
{
  return m_cmdSpecs.count(cmdName) > 0;
}

QList<QString> CommandRegistry::getCommandSpecs(
  const QString &cmdName) 
{
  if(!isCommandRegistered(cmdName))
    FabricException::Throw( 
      "CommandRegistry::getCommandSpecs",
      "cannot create command '" + cmdName + "', it's not registered"
      );

  return m_cmdSpecs[cmdName];
}

BaseCommand* CommandRegistry::createCommand(
  const QString &cmdName) 
{  
  if(!isCommandRegistered(cmdName))
    FabricException::Throw( 
      "CommandRegistry::createCommand",
      "cannot create command '" + cmdName + "', it's not registered"
      );

  try
  {
    QList<QString> spec = getCommandSpecs(cmdName);
    
    if(spec[1] == COMMAND_CPP) 
    {
      Factory *factory = Util::BaseFactoryRegistry::getFactory(
        cmdName);

      BaseCommand* cmd = (BaseCommand*)factory->create(); 
      if(cmd == 0)
        FabricException::Throw(
          "CommandRegistry::createCommand",
          "resulting command is null" 
          );

      void *userData = factory->getUserData();
    
      cmd->registrationCallback(
        cmdName,
        userData);

      return cmd;
    }
  }
    
  catch(FabricException &e) 
  {
    FabricException::Throw(
      "CommandRegistry::createCommand",
      e.what()
      );
  }
 
  return 0;
}

QString CommandRegistry::getContent()
{
  QString res = "--> CommandRegistry:\n";
  QMapIterator< QString, QList<QString> > specsIt(m_cmdSpecs);

  while(specsIt.hasNext()) 
  {
    specsIt.next();
    QString name = specsIt.key();
    QList<QString> spec = specsIt.value();

    res += QString(
      "["+ name + "] type:" + spec[0] + ", implType:" + spec[1] + "\n"
    );
  }

  return res;
}

void CommandRegistry::commandIsRegistered(
  const QString &cmdName,
  const QString &cmdType,
  const QString &implType) 
{
  // sets the command specs
  QList<QString> spec;
  spec.append(cmdType);
  spec.append(implType);
  m_cmdSpecs[cmdName] = spec;
    
  // inform a command has been registered.
  emit commandRegistered(
    cmdName,
    cmdType,
    implType);
}
