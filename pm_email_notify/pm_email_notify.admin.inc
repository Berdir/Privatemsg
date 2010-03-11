<?php
// $Id$

/**
 * @file
 * Administration menu callbacks for pm_email_notify.module.
 */


/**
 * Menu callback for administration settings.
 */
function pm_email_notify_admin_settings_form() {
  $form['pm_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privatemsg e-mail notification'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['pm_email']['pm_email_notify_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify users of new private messages by default'),
    '#default_value' => variable_get('pm_email_notify_default', TRUE),
    '#weight' => 0,
  );

  $form['pm_email']['pm_email_notify_desc'] = array(
    '#type' => 'markup',
    '#value' => t('Customize the email messages sent to users upon receipt of a new private message. <br />Available variables are: !author, !author_uid, !pm_subject, !pm_body, !thread, !site, !login_url, !uri, !uri_brief, !message (URL) and !settings (URL). <br />To translate these messages use the i18nstrings.module and translate them trough the <a href="!translate_url">Translate interface</a> ("Privatemsg Email Notification" text group).', array('!translate_url' => url('admin/build/translate/search'))),
    '#weight' => 1,
  );

  $form['pm_email']['pm_email_notify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of notification messages'),
    '#default_value' => _pm_email_notify_text('subject', NULL, FALSE),
    '#weight' => 2,
  );

  $form['pm_email']['pm_email_notify_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of notification messages'),
    '#default_value' => _pm_email_notify_text('body', NULL, FALSE),
    '#weight' => 3,
  );
  return system_settings_form($form);
}