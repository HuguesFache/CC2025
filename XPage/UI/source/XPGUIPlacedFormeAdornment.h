#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace XPageUI {

	/// <summary>
	/// Description rÈsumÈe de XPGUIPlacedFormeAdornment
	///
	/// AVERTISSEMENT†: si vous modifiez le nom de cette classe, vous devrez modifier la
	///          propriete 'Nom du fichier de ressources' de l'outil de compilation de ressource managee
	///          pour tous les fichiers .resx dont depend cette classe. Dans le cas contraire,
	///          les concepteurs ne pourront pas interagir correctement avec les ressources
	///          localisees associees a ce formulaire.
	/// </summary>
	public ref class XPGUIPlacedFormeAdornment : public System::Windows::Forms::Form
	{
	public:
		XPGUIPlacedFormeAdornment(void)
		{
			InitializeComponent();
			//
			//TODO†: ajoutez ici le code du constructeur
			//
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisÈes.
		/// </summary>
		~XPGUIPlacedFormeAdornment()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Variable nÈcessaire au concepteur.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// MÈthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette mÈthode avec l'Èditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->Size = System::Drawing::Size(300,300);
			this->Text = L"XPGUIPlacedFormeAdornment";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        }
#pragma endregion
	};
}
