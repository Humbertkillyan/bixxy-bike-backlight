---
name: 🐛 Bug Report
description: Signaler un bug ou problème
labels: ["bug"]

body:
  - type: markdown
    attributes:
      value: |
        Merci de rapporter ce bug! Remplissez les informations ci-dessous.

  - type: input
    id: title
    attributes:
      label: Titre
      description: Résumé court du problème
      placeholder: "[Module] Brève description"
    validations:
      required: true

  - type: textarea
    id: description
    attributes:
      label: Description
      description: Décrivez le problème en détail
      placeholder: "Expliquez ce qui ne fonctionne pas..."
    validations:
      required: true

  - type: textarea
    id: reproduction
    attributes:
      label: Étapes pour reproduire
      description: Comment reproduire le bug?
      placeholder: |
        1. Initialiser le module...
        2. Appeler la fonction...
        3. Observer le comportement incorrect
    validations:
      required: true

  - type: textarea
    id: expected
    attributes:
      label: Comportement attendu
      description: Qu'est-ce qui devrait se passer?
    validations:
      required: true

  - type: textarea
    id: environment
    attributes:
      label: Environnement
      description: Détails du setup
      placeholder: |
        - **Platform**: Arduino Uno / Mega / ESP32
        - **IDE**: Arduino IDE / PlatformIO
        - **Librairies**: Versions utilisées
        - **Hardware**: Modules connectés
    validations:
      required: true

  - type: textarea
    id: logs
    attributes:
      label: Logs / Erreurs
      description: Copiez les messages d'erreur
      render: text

  - type: checkboxes
    id: checklist
    attributes:
      label: Checklist
      options:
        - label: J'ai lu la documentation
        - label: C'est un nouveau bug (pas déjà rapporté)
        - label: J'ai essayé disponible sur main
